///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		ARdrone++: library
//			Author: Pablo R.S. (A.k.a. Bardo91)
//			Date:	2015-APR-06
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	UdpSocket is an UDP socket implementation that fits ARdrone needs.

#include "UdpSocket.h"
#include <cassert>
#include <cstring>
#include <iostream>
namespace ardronepp{
	//-----------------------------------------------------------------------------------------------------------------
	UdpSocket::UdpSocket(const std::string &_droneIp, unsigned _port){
		#if defined(_WIN32) // Initialize WSA
			WSAData wsaData;
			WSAStartup(MAKEWORD(1, 1), &wsaData);
		#endif

		// Create an udp socket socket
		assert( (mSocket = socket(AF_INET, SOCK_DGRAM, 0)) != INVALID_SOCKET);

		configureAddrs(_droneIp, _port);

		// Bind socket
		assert(bind(mSocket, (sockaddr*)&mClientAddr, sizeof(mClientAddr)) != SOCKET_ERROR);

		// Enable re-use address option
		int reuse = 1;
		assert(setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) != SOCKET_ERROR);
		
	}

	//-----------------------------------------------------------------------------------------------------------------
	void UdpSocket::configureAddrs(const std::string &_droneIp, unsigned _port){
		// Clear address structures
		memset(&mDroneAddr, 0, sizeof(mDroneAddr));
		memset(&mClientAddr, 0, sizeof(mClientAddr));

		// Configure erver addres
		mDroneAddr.sin_family = AF_INET;
		mDroneAddr.sin_port = htons((u_short)_port);
		#if defined(_WIN32)
			mDroneAddr.sin_addr.s_addr = inet_addr(_droneIp.c_str());
		#elif defined (__linux__)
			inet_aton(_droneIp.c_str(), &mDroneAddr.sin_addr);
		#endif

		// Configure client addres
		mClientAddr.sin_family = AF_INET;
		mClientAddr.sin_port = htons(0);
		mClientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	}

	//-----------------------------------------------------------------------------------------------------------------
	int UdpSocket::send(const std::string &_msg){
		assert(mSocket != INVALID_SOCKET);

		int n = sendto(mSocket, _msg.c_str(), _msg.size(), 0, (sockaddr*)&mDroneAddr, sizeof(mDroneAddr));
		

		return n < 1 ? 0 : n;
	}

	//-----------------------------------------------------------------------------------------------------------------
	int UdpSocket::receive(char *_buffer, const unsigned _size){
		assert(mSocket != INVALID_SOCKET);

		sockaddr_in addr;
		socklen_t len = sizeof(addr);
		return  recvfrom(mSocket, _buffer, _size, 0, (sockaddr*)&addr, &len);
	}

}	//	namespace ardronepp


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		ARdrone++: library
//			Author: Pablo R.S. (A.k.a. Bardo91)
//			Date:	2015-APR-06
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	UdpSocket is an UDP socket implementation that fits ARdrone needs.


#ifndef ARDRONEPP_CORE_COMM_UDP_SOCKET_H_
#define ARDRONEPP_CORE_COMM_UDP_SOCKET_H_

#if defined(_WIN32)
	#define WIN32_LEAN_AND_MEAN

	#include <Windows.h>
	#include <winsock2.h>
	#include <ws2tcpip.h>

	#pragma comment (lib, "Ws2_32.lib")
	#pragma comment (lib, "Mswsock.lib")
	#pragma comment (lib, "AdvApi32.lib")
#elif defined(__linux__)
	#include <unistd.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netdb.h>
	#include <arpa/inet.h>
	
	typedef int SOCKET;
	const int INVALID_SOCKET = -1;
	const int SOCKET_ERROR = -1;

	#define closesocket(SOCKET) close(SOCKET)
#endif

#include <string>

namespace ardronepp{

	class UdpSocket{
	public:	// Public interface
		UdpSocket(const std::string &_droneIp, unsigned _port);

		int send(const std::string &_msg);
		int receive(char *_buffer,const unsigned _size);

	private:	// Private methods
		void configureAddrs(const std::string &_droneIp, unsigned _port);

	private:	// Private members
		SOCKET mSocket;
		sockaddr_in mDroneAddr, mClientAddr;

	};	//	 Class UdpSocket
}	//	namespace ardronepp

#endif	//	ARDRONEPP_CORE_COMM_UDP_SOCKET_H_

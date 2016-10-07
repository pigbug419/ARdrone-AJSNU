///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		ARdrone++: library
//			Author: Pablo R.S. (A.k.a. Bardo91)
//			Date:	2015-APR-18
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	


#ifndef ARDRONE_DRONE_CONTROLLER_H_
#define ARDRONE_DRONE_CONTROLLER_H_

#include "../core/comm/UdpSocket.h"

namespace ardronepp{
	class Controller{
	public:
		Controller();
		void send(std::string _msg);

		void setGroundReference();

		void takeOff();
		void land();
		void hovering();
		void spin(float _yawSpeed);					// _yawSpeed	= [-1, 1]	left - right
		void lift(float _vSpeed);					// _vSpeed		= [-1, 1]	ip - down
		void translate(float _pitch, float _roll);	// _pitch		= [-1, 1]	front - back
													// _roll		= [-1, 1]	left - right

	private:
		UdpSocket mControlSocket;

		unsigned	mCommandCounter;

	};
}	//	namespace ardronepp


#endif // ARDRONE_DRONE_CONTROLLER_H_

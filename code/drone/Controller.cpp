///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		ARdrone++: library
//			Author: Pablo R.S. (A.k.a. Bardo91)
//			Date:	2015-APR-18
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	


#include "Controller.h"

#include <cstring>
#include <sstream>

namespace ardronepp{
	Controller::Controller() : mControlSocket("192.168.1.1", 5556), mCommandCounter(1){}

	//---------------------------------------------------------------------------------------------------------------------
	void Controller::send(std::string _msg){
		std::stringstream ATstream;
		ATstream << _msg.substr(0, _msg.find("%d")) << mCommandCounter++ << _msg.substr(_msg.find("%d") + 2, _msg.size());
		mControlSocket.send(ATstream.str());
	}

	//---------------------------------------------------------------------------------------------------------------------
	void Controller::setGroundReference(){
		// assert(not flying);
		std::stringstream ATstream;
		ATstream << "AT*FTRIM=" << mCommandCounter++ << "\r";

		mControlSocket.send(ATstream.str());
	}

	//---------------------------------------------------------------------------------------------------------------------
	void Controller::takeOff(){
		std::stringstream ATstream;
		ATstream << "AT*REF=" << mCommandCounter++ << ",290718464\r";

		mControlSocket.send(ATstream.str());
	}

	//---------------------------------------------------------------------------------------------------------------------
	void Controller::land(){
		std::stringstream ATstream;
		ATstream << "AT*REF=" << mCommandCounter++ << ",290717696\r";

		mControlSocket.send(ATstream.str());
	}

	//---------------------------------------------------------------------------------------------------------------------
	void Controller::hovering(){
		std::stringstream ATstream;
		ATstream << "AT*PCMD=" << mCommandCounter++ << ",1,0,0,0,0\r";

		mControlSocket.send(ATstream.str());
	}

	//---------------------------------------------------------------------------------------------------------------------
	void Controller::spin(float _yawSpeed){
		int yawSpeed;
		memcpy(&yawSpeed, &_yawSpeed, sizeof(float));

		std::stringstream ATstream;
		ATstream << "AT*PCMD=" << mCommandCounter++ << ",1,0,0,0," << yawSpeed << "\r";

		mControlSocket.send(ATstream.str());
	}

	//---------------------------------------------------------------------------------------------------------------------
	void Controller::lift(float _vSpeed){
		int vSpeed;
		memcpy(&vSpeed, &_vSpeed, sizeof(float));

		std::stringstream ATstream;
		ATstream << "AT*PCMD=" << mCommandCounter++ << ",1,0,0," << vSpeed << ",0\r";

		mControlSocket.send(ATstream.str());
	}

	//---------------------------------------------------------------------------------------------------------------------
	void Controller::translate(float _pitch, float _roll){
		_pitch = -_pitch;	// Negative value get nose down, so move forward.

		int pitch, roll;
		memcpy(&pitch, &_pitch, sizeof(float));
		memcpy(&roll, &_roll, sizeof(float));

		std::stringstream ATstream;
		ATstream << "AT*PCMD=" << mCommandCounter++ << ",1," << roll << "," << pitch << ",0,0\r";

		mControlSocket.send(ATstream.str());
	}

}	//	namespace ardronepp
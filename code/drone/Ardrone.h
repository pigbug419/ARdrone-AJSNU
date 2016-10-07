///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		ARdrone++: library
//			Author: Pablo R.S. (A.k.a. Bardo91)
//			Date:	2015-APR-06
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Main Ardrone class

#ifndef ARDRONEPP_ARDRONE_H_

#include "../core/comm/UdpSocket.h"
#include "navigation/Telemetry.h"
#include "Controller.h"

namespace ardronepp{

	class Ardrone{
	public:
		Ardrone();
		Controller & control();
		Telemetry & telemetry();
	private:
		void initConnections();

	private:
		Telemetry	mTelemeter;
		Controller	mController;
	};	//	class Ardrone

}	//	namespace ardronepp

#endif // ARDRONEPP_ARDRONE_H_


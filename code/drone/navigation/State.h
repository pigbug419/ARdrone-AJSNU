///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		ARdrone++: library
//			Author: Pablo R.S. (A.k.a. Bardo91)
//			Date:	2015-APR-21
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Ardrone state class



#ifndef ARDRONE_DRONE_NAVIGATION_STATE_H_
#define ARDRONE_DRONE_NAVIGATION_STATE_H_

enum eStateMask {
	fly						= 1U <<  0, // FLY MASK                  : (0) Ardrone is landed, (1) Ardrone is flying
	video					= 1U <<  1, // VIDEO MASK                : (0) Video disable, (1) Video enable
	vision					= 1U <<  2, // VISION MASK               : (0) Vision disable, (1) Vision enable
	control					= 1U <<  3, // CONTROL ALGO              : (0) Euler angles control, (1) Angular speed control
	altitude				= 1U <<  4, // ALTITUDE CONTROL ALGO     : (0) Altitude control inactive (1) Altitude control active
	userFeedBackStart		= 1U <<  5, // USER feedback             :     Start button state 
	command					= 1U <<  6, // Control command ACK       : (0) None, (1) One received
	camera					= 1U <<  7, // CAMERA MASK               : (0) Camera not ready, (1) Camera ready
	travelling				= 1U <<  8, // Travelling mask           : (0) Disable, (1) Enable
	usb						= 1U <<  9, // USB key                   : (0) Usb key not ready, (1) Usb key ready
	navdataDemo				= 1U << 10, // Navdata demo              : (0) All navdata, (1) Only navdata demo
	booStrap				= 1U << 11, // Navdata bootstrap         : (0) Options sent in all or demo mode, (1) No navdata options sent
	motors					= 1U << 12, // Motors status             : (0) Ok, (1) Motors problem
	comLost					= 1U << 13, // Communication Lost        : (1) Com problem, (0) Com is ok
	vBat					= 1U << 15, // VBat low                  : (1) Too low, (0) Ok
	userEmergencyLanding	= 1U << 16, // User Emergency Landing    : (1) User EL is ON, (0) User EL is OFF
	timerElapsed			= 1U << 17, // Timer elapsed             : (1) Elapsed, (0) Not elapsed
	angelOutOfRange			= 1U << 19, // Angles                    : (0) Ok, (1) Out of range
	ultrasound				= 1U << 21, // Ultrasonic sensor         : (0) Ok, (1) Deaf
	cutOutDetection			= 1U << 22, // Cutout system detection   : (0) Not detected, (1) Detected
	picVersion				= 1U << 23, // PIC Version number OK     : (0) A bad version number, (1) Version number is OK */
	atCodecThreadOn			= 1U << 24, // ATCodec thread ON         : (0) Thread OFF (1) thread ON
	navdataThreadOn			= 1U << 25, // Navdata thread ON         : (0) Thread OFF (1) thread ON
	videoThreadOn			= 1U << 26, // Video thread ON           : (0) Thread OFF (1) thread ON
	acquisitionThreadOn		= 1U << 27, // Acquisition thread ON     : (0) Thread OFF (1) thread ON
	controlWatchDog			= 1U << 28, // CTRL watchdog             : (1) Delay in control execution (> 5ms), (0) Control is well scheduled
	adcWatchDog				= 1U << 29, // ADC Watchdog              : (1) Delay in uart2 dsr (> 5ms), (0) Uart2 is good
	comWatchDog				= 1U << 30, // Communication Watchdog    : (1) Com problem, (0) Com is ok
	emergecy				= 1U << 31  // Emergency landing         : (0) No emergency, (1) Emergency
};

//---------------------------------------------------------------------------------------------------------------------
namespace ardronepp{
	class State{
	public:
		operator unsigned*();

		bool flying();
		bool eulerAngleControl();
		bool navdataDemo();
		bool emergencyMode();
		bool lowBattery();

	private:
		unsigned mState;
	};
}	//	namespace ardronepp
#endif // ARDRONE_DRONE_NAVIGATION_STATE_H_

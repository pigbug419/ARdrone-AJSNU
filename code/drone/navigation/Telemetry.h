///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		ARdrone++: library
//			Author: Pablo R.S. (A.k.a. Bardo91)
//			Date:	2015-APR-07
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Telemetry


#ifndef ARDRONEPP_DRONE_NAVIGATION_TELEMETRY_H_
#define ARDRONEPP_DRONE_NAVIGATION_TELEMETRY_H_

#include "../Controller.h"
#include "../../core/comm/UdpSocket.h"
#include "NavigationData.h"
#include "State.h"

#include <array>
#include <mutex>
#include <string>
#include <thread>

namespace ardronepp{
	class Telemetry{
	public:		// Public interface
		Telemetry(Controller *_controller);
		~Telemetry();

		std::array<std::array<float, 3>, 3>		orientation();
		std::array<float, 3>					position();
		std::array<float, 3>					speed();

		State									state();

	private:	// Private methods
		void acquisitionCallback();
		void update();

	private:	// Private members
		Controller					*mArdroneController;
		UdpSocket					mTelemetrySocket;
		bool						mAcquire;
		std::thread					*mAcquisitionThread;
		std::mutex					mSecureAcquisition;

		//	ARdrone telemetry
		unsigned int				mMsgHeader;
		State						mState;
		unsigned int				mSequenceNumber;

		navdata::DemoData			mBasicData;

		navdata::TimeStamp			mTimeStamp;
		navdata::EulerMat			mOrientation;
		navdata::Point3				mPosition;
		navdata::Altitude			mAltitude;
		navdata::Speeds				mSpeeds;

		navdata::RawImu				mRawImu;
		navdata::RawPressure		mRawPressure;
		navdata::PressureFiltered	mPressureFiltered;
		navdata::Magneto			mMagneto;
		navdata::Wind				mWind;

		navdata::WatchDog			mWatchDog;
		navdata::Wifi				mWifiStatus;
		navdata::CheckSum			mCheckSum;

	};
}


#endif // ARDRONEPP_DRONE_NAVIGATION_TELEMETRY_H_

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		ARdrone++: library
//			Author: Pablo R.S. (A.k.a. Bardo91)
//			Date:	2015-APR-06
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Time tools


#include "TimeTools.h"
#include <cassert>

namespace ardronepp{
	STime* STime::sTime = nullptr;	//	Static data initialization.

	//------------------------------------------------------------------------------------------------------------------
	STime* STime::get() {
		if (sTime == nullptr)
			init();
		return sTime;
	}
	
	//------------------------------------------------------------------------------------------------------------------
	void STime::init() {
		if (sTime == nullptr)
			sTime = new STime();
	}

	//------------------------------------------------------------------------------------------------------------------
	void STime::end() {
		assert(sTime != nullptr);
		delete sTime;
		sTime = nullptr;
	}

	//------------------------------------------------------------------------------------------------------------------
	STime::STime() {
		#if defined (__linux__)
			gettimeofday(&mInitTime, 0);			// Get current time
		#elif defined (WIN32)
			QueryPerformanceCounter(&mInitTime);	// Get initial time
		#endif
	}

	//------------------------------------------------------------------------------------------------------------------
	double STime::getTime() {
		if (!sTime)
			init();

		#if defined (__linux__)
		// Get current time
			timeval currentTime;
			gettimeofday(&currentTime, 0);
			return double(currentTime.tv_sec - mInitTime.tv_sec) + double(currentTime.tv_usec - mInitTime.tv_usec) / 1000000;
		#elif defined (_WIN32)
			// Get current time
			LARGE_INTEGER largeTicks;
			QueryPerformanceCounter(&largeTicks);
			unsigned currTime = largeTicks.LowPart;
			// Convert time difference to seconds
			LARGE_INTEGER frequency;
			QueryPerformanceFrequency(&frequency);
			return (double(currTime) / double(frequency.LowPart)) -
				(double(mInitTime.LowPart) / double(frequency.LowPart));
		#endif 
	}
	//------------------------------------------------------------------------------------------------------------------
	void STime::delay(const unsigned _seconds){		
		double t0 = getTime();
		while (getTime() - t0 < _seconds){
			//	 Intentionally blank
		}
	}
	
	//------------------------------------------------------------------------------------------------------------------
	void STime::mDelay(const unsigned _millis){
		double t0 = getTime();
		while ((getTime() - t0)*100 < _millis){
			//	 Intentionally blank
		}
	}

}	//	namespace ardronepp
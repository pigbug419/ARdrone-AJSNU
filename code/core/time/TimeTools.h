///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		ARdrone++: library
//			Author: Pablo R.S. (A.k.a. Bardo91)
//			Date:	2015-APR-06
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Time tools


#ifndef ARDRONEPP_CORE_TIME_TIME_H_
#define ARDRONEPP_CORE_TIME_TIME_H_

#if defined(__linux__)
#include <sys/time.h>
#elif defined(_WIN32)
#include <Windows.h>
#endif

namespace ardronepp{
	class STime {
	// Static interface
	public:
		static STime*	get			();
		static void		init		();
		static void		end			();

	private:
		static	STime*				sTime;
		
	// Class interface
	public:
		double	getTime		();
		void	delay		(const unsigned _seconds);
		void	mDelay		(const unsigned _millis);

	private:
		STime();
		#if defined(__linux__)
			timeval			mInitTime;
		#elif defined (_WIN32)
			LARGE_INTEGER	mInitTime;
		#endif
	};
}	//	namespace ardronepp


#endif	//	ARDRONEPP_CORE_TIME_TIME_H_
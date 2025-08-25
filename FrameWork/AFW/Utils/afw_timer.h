#pragma once

//#include "ofConstants.h"
//#include "afw_utils.h"

#include <windows.h>
#include <chrono>

#define NANOS_PER_SEC 1000000000ll

class ofTimer {
public:

	/// \brief Constructs the default ofTimer().
	ofTimer():nanosPerPeriod(0)
		//#ifdef TARGET_WIN32
		,hTimer(CreateWaitableTimer(nullptr, TRUE, nullptr))
		//#endif
		{
		}

	/// \brief  Reset the starting instant of the periodic event to now.
	void reset(){
		//#if defined(TARGET_WIN32)
				GetSystemTimeAsFileTime((LPFILETIME)&nextWakeTime);
		//#else
		//		nextWakeTime = ofGetCurrentTime();
		//#endif
		calculateNextPeriod();
	}

	/// \brief Sets the frequency in \p nanoseconds that events happen.
	/// 
	/// The timer will begin calculating the events beginning the moment
	/// the function is called.
	/// \param nanoseconds The number of nanoseconds between events.
	void setPeriodicEvent(uint64_t nanoseconds) {
		nanosPerPeriod = std::chrono::nanoseconds(nanoseconds);
		reset();
	}

	/// \brief Sleep this thread until the next periodic event.
	void waitNext() {
		//#if (defined(TARGET_LINUX) && !defined(TARGET_RASPBERRY_PI_LEGACY))
		//		timespec remainder = { 0,0 };
		//		timespec wakeTime = nextWakeTime.getAsTimespec();
		//		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &wakeTime, &remainder);
		//#elif defined(TARGET_WIN32)
				WaitForSingleObject(hTimer, INFINITE);
		//#else
			//auto now = ofGetCurrentTime();
			//if (now < nextWakeTime) {
			//	auto waitNanos = nextWakeTime - now;
			//	timespec waittime = (ofTime() + waitNanos).getAsTimespec();
			//	timespec remainder = { 0,0 };
			//	nanosleep(&waittime, &remainder);
			//}
		//#endif
		calculateNextPeriod();
	}

private:
	void calculateNextPeriod() {
//		#if defined(TARGET_WIN32)
			nextWakeTime.QuadPart += nanosPerPeriod.count() / 100;
			LARGE_INTEGER now;
			GetSystemTimeAsFileTime((LPFILETIME)&now);
			if (nextWakeTime.QuadPart < now.QuadPart) {
				reset();
			}
		//	else {
		//		SetWaitableTimer(hTimer, &nextWakeTime, 0, nullptr, nullptr, 0);
		//	}
		//#else
		//	nextWakeTime += nanosPerPeriod;
		//	auto now = ofGetCurrentTime();
		//	if (nextWakeTime < now) {
		//		reset();
		//	}
		//#endif
	}

	std::chrono::nanoseconds nanosPerPeriod;
	//#if defined(TARGET_WIN32)
		LARGE_INTEGER nextWakeTime;
		HANDLE hTimer;
	//#else
	//	ofTime nextWakeTime;
	//#endif
};

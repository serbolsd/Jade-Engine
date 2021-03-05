#include "jdTimer.h"

namespace jdEngineSDK {

		Timer::Timer()	{
				reset();
		}

		void 
		Timer::reset()	{
				m_startTime = m_highResClock.now();
		}

		uint64 
		Timer::getMilliseconds() const	{
				auto newTime = m_highResClock.now();
				std::chrono::duration<double> dur = newTime - m_startTime;
				return duration_cast<std::chrono::milliseconds>(dur).count();
		}
		
		uint64 
		Timer::getMicroseconds() const {
				auto newTime = m_highResClock.now();
				std::chrono::duration<double> dur = newTime - m_startTime;
				return duration_cast<std::chrono::microseconds>(dur).count();
		}
		
		float 
		Timer::getSeconds() const	{
				auto newTime = m_highResClock.now();
				std::chrono::duration<double> dur = newTime - m_startTime;
				return duration_cast<std::chrono::microseconds>(dur).count() / 1000000;
		}

		uint64 
		Timer::getStartMilliseconds() const	{
				std::chrono::nanoseconds startTimeNs = m_startTime.time_since_epoch();
				return duration_cast<std::chrono::milliseconds>(startTimeNs).count();
		}
}
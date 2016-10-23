#ifndef __TIMER_INCLUDED__
#define __TIMER_INCLUDED__

/**
* @author Manwe
*
* Time management class in order to measure elapsed time and avoid time outs
* The Timer class uses the appropriated method to get the time in function of the platform. It is not executed in a separated thread, you must use the timeCheck method in order to verify the timeout has not been reached during the execution of your computation
*/

#include <chrono>
#include <stdexcept>
#include <string>

namespace competitive {
	namespace programming {
		namespace timemanagement {

			struct TimeoutException : public std::exception
			{
				TimeoutException(): std::exception()
				{}
			};

			class Timer
			{
			public:
				Timer() : m_startTime(), m_timeOut(), m_started(false)
				{
				};

				/**
				* Start the timer.
				* If the timer is already started, will simply define the timeout as now + duration
				* A call to this method is mandatory if you want the timeCheck method to throws timeout exceptions
				*
				* @param durationInMilliseconds
				* 		The duration in milliseconds from now until which the timeCheck method will throws Timeoutexceptions
				*/
				void startTimer(double durationInMilliseconds)
				{
					m_startTime = std::chrono::high_resolution_clock::now();
					m_timeOut = std::chrono::nanoseconds((long long)(durationInMilliseconds*1000000));
					m_started = true;
				};

				/**
				* Verify if the timeout has been reached. If yes, throws a TimeoutException
				* will not throw anything if the timer has never been started.
				* @throws TimeoutException
				*/
				void timeCheck() const {
					if (m_started) {
						std::chrono::nanoseconds timeSpent = std::chrono::high_resolution_clock::now() - m_startTime;
						if (timeSpent > m_timeOut) {
							throw TimeoutException();
						}
					}
				}

				/**
				* @return
				*  the number of nanoseconds between last time the timer has been started and now
				*  start the timer before using this method!
				*/
				std::chrono::nanoseconds currentTimeTakenInNanoSeconds() const {
					return std::chrono::high_resolution_clock::now() - m_startTime;
				}

			private:
				std::chrono::high_resolution_clock::time_point m_startTime;
				std::chrono::nanoseconds m_timeOut;
				bool m_started;
			};

			std::ostream& operator<<(std::ostream &os, const Timer& timer)
			{
				os << "Time taken: " << timer.currentTimeTakenInNanoSeconds().count() << "ns";
				return os;
			}
		}
	}
}

#endif
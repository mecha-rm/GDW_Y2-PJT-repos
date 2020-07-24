/*
 * References:
 *	http://www.cplusplus.com/reference/chrono/duration/
*/


#pragma once
#include <chrono>
#include <time.h>

namespace util
{
	namespace time
	{
		class Timer
		{
		public:
			// default constructor. Call 'start' to start th timer.
			Timer();

			// starts the timer if 'true' is passed.
			Timer(bool startTimer);

			// starts the timer.
			void start();

			// pauses the timer.
			// void pause();
			// 
			// // unpauses the timer.
			// void unpause();

			// ends the timer.
			void stop();

			// gets the duration in nanoseconds.
			// if the timer hasn't been stopped, then it returns the amount of time since the timer was last started.
			// if the timer hasn't been started once, (0) is returned.
			double getDurationInNanoseconds();

			// gets the duration in nanoseconds.
			// if the timer hasn't been stopped, then it returns the amount of time since the timer was last started.
			// if the timer hasn't been started once, (0) is returned.
			double getDurationInMicroseconds();

			// gets the duration in nanoseconds.
			// if the timer hasn't been stopped, then it returns the amount of time since the timer was last started.
			// if the timer hasn't been started once, (0) is returned.
			double getDurationInMilliseconds();

			// gets the duration in nanoseconds.
			// if the timer hasn't been stopped, then it returns the amount of time since the timer was last started.
			// if the timer hasn't been started once, (0) is returned.
			double getDurationInSeconds();

			// gets the duration in nanoseconds.
			// if the timer hasn't been stopped, then it returns the amount of time since the timer was last started.
			// if the timer hasn't been started once, (0) is returned.
			double getDurationInMinutes();

			// gets the current time in milliseconds. 
			// if the timer hasn't been stopped, then it returns the amount of time since the timer was last started.
			// if the timer hasn't been started once, (0) is returned.
			double getDurationInHours();

		private:
			// if 'true', then the timer is running.
			bool running = false;

			// if 'true', then the timer is paused.
			bool paused = false;

			// the starting time.
			std::chrono::steady_clock::time_point startTime;

			// the ending time.
			std::chrono::steady_clock::time_point endTime;

			// the time that the program was paused
			std::chrono::steady_clock::time_point pauseTime;

			// the pause time (in count) for the timer.
			double pauseTimeCount = 0;

			// the time 

		protected:
		};
	}
}
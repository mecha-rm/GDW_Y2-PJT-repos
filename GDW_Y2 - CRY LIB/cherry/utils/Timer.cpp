#include "Timer.h"

// creates a timer, but doesn't start it.
util::time::Timer::Timer() : Timer(false)
{

}

// starts the timer if 'true' is passed.
util::time::Timer::Timer(bool startTimer)
{
	startTime = std::chrono::high_resolution_clock::now();
	endTime = startTime;

	if (startTimer)
		start();
}

// starts the timer.
void util::time::Timer::start()
{
	running = true;
	paused = false;
	startTime = std::chrono::high_resolution_clock::now();
}

// pauses the timer.
// void util::time::Timer::pause()
// {
// 	// the timer has been paused, so it's not running.
// 	running = false;
// 
// 	// gets the current time since it's now paused.
// 	pauseTime = std::chrono::high_resolution_clock::now();
// }

// unpauses the timer.
// void util::time::Timer::unpause()
// {
// 	// gets hte current time.
// 	auto now = std::chrono::high_resolution_clock::now();
// 
// 
// 	// TODO: unpause functionality.
// }

// ends the timer
void util::time::Timer::stop()
{
	running = false;
	paused = false;
	endTime = std::chrono::high_resolution_clock::now();
}

// gets the duration in nanoseconds.
double util::time::Timer::getDurationInNanoseconds()
{
	// if the timer is currently running, end time is updated to be the current time.
	if(running)
		endTime = std::chrono::high_resolution_clock::now();

	// gets the current time
	return std::chrono::duration<double, std::nano>(endTime - startTime).count();
}

// gets the duration in microseconds.
double util::time::Timer::getDurationInMicroseconds()
{
	// if the timer is currently running, end time is updated to be the current time.
	if (running)
		endTime = std::chrono::high_resolution_clock::now();

	// gets the current time
	return std::chrono::duration<double, std::micro>(endTime - startTime).count();
}

// gets the duration in milliseconds.
double util::time::Timer::getDurationInMilliseconds()
{
	// if the timer is currently running, end time is updated to be the current time.
	if (running)
		endTime = std::chrono::high_resolution_clock::now();

	// gets the current time
	return std::chrono::duration<double, std::milli>(endTime - startTime).count();
}

// gets the duration in seconds.
double util::time::Timer::getDurationInSeconds()
{
	// if the timer is currently running, end time is updated to be the current time.
	if (running)
		endTime = std::chrono::high_resolution_clock::now();

	// gets the current time
	return std::chrono::duration<double, std::ratio<1, 1>>(endTime - startTime).count();
}

// gets the duration in minutes.
double util::time::Timer::getDurationInMinutes()
{
	// if the timer is currently running, end time is updated to be the current time.
	if (running)
		endTime = std::chrono::high_resolution_clock::now();

	// gets the current time
	return std::chrono::duration<double, std::ratio<60, 1>>(endTime - startTime).count();
}

// gets the duration in hours.
double util::time::Timer::getDurationInHours()
{
	// if the timer is currently running, end time is updated to be the current time.
	if (running)
		endTime = std::chrono::high_resolution_clock::now();

	// gets the current time
	return std::chrono::duration<double, std::ratio<3600, 1>>(endTime - startTime).count();
}
#include "Instrumentation.h"
#include <thread>
#include <toolkit/Logging.h>
#include <iostream>

cherry::ProfileTimer::ProfileTimer(const std::string& name, bool start) {
	myName = name;
	isStopped = !start;

	// current time is starting time
	myStartTime = time_now();
}

cherry::ProfileTimer::~ProfileTimer() {
	Stop();
}

// restarts the timer
void cherry::ProfileTimer::Restart() {
	isStopped = false;

	// reset initial time
	myStartTime = time_now();
}

// stops the timer
void cherry::ProfileTimer::Stop() {
	if (!isStopped) {
		const Timepoint endTime = time_now();
		isStopped = true;
		// Create our profiling result
		ProfilingResult result;
		result.Name = myName;
		// std::chrono is dumb, this is how we get our actual microseconds out (since Jan 1, 1970)
		result.Begin = myStartTime.time_since_epoch().count();
		result.End = endTime.time_since_epoch().count();
		// C++ is dumb, this is how we get our thread ID
		result.ThreadID = std::hash<std::thread::id>()(std::this_thread::get_id());
		result.Args = ""; // TODO, support arguments to timers
		// Write the result to our JSON file
		ProfilingSession::WriteResult(result);
	}
}
// taken from the GOOP framework

#pragma once
#include <chrono>
#include <string>
#include "JsonProfiling.h"

namespace cherry
{
	typedef std::chrono::high_resolution_clock TimeClock;

	typedef std::chrono::microseconds TimeResolution;

	typedef std::chrono::time_point<TimeClock, TimeResolution> Timepoint;

	// gets the current time
	inline Timepoint time_now() { return std::chrono::time_point_cast<TimeResolution>(TimeClock::now()); }

	class ProfileTimer
	{
	public:
		ProfileTimer(const std::string& name, bool start = true);

		~ProfileTimer();

		void Stop();

		void Restart();
	private:
		Timepoint myStartTime;

		bool isStopped;

		std::string myName;
	};
}
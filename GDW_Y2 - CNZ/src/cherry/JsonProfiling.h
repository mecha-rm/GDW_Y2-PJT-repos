#pragma once
#include <string>
#include <fstream>

namespace cherry
{
	/*
	 * Represents a single timing event, with a beginning and end time
	 */
	struct ProfilingResult {
		/*
		 * The name of the event to display in the trace tool
		 */
		std::string Name;
		/*
		 * Any arguments to display in the trace tool associated with this event
		 */
		std::string Args;
		/*
		 * The timestamps that indicate the start and end times, in microseconds
		 */
		uint64_t    Begin, End;
		/*
		 * The ID of the thread that dispatched this event (unused for now)
		 */
		uint32_t    ThreadID;
	};

	/*
	 * Singleton class for writing profiling events to a JSON file for later viewing
	 */
	class ProfilingSession
	{
	public:
		/*
		 * Starts a new profiling session, and ends any existing session
		 * @param outPath The path to write the JSON to
		 */
		static void Start(const std::string& outPath);
		/*
		 * Ends the profiling session and closes the file
		 */
		static void End();

		/*
		 * Writes a profiling result to the output
		 * @param result The result to write
		 */
		static void WriteResult(const ProfilingResult& result);

		~ProfilingSession();

	private:
		ProfilingSession(const std::string& outPath);

		std::ofstream myOutStream;
		size_t        myProfileCount;

		void _WriteHeader();
		void _WriteFooter();
		void _WriteResult(const ProfilingResult& result);

		// The singleton instance
		static ProfilingSession* __Session;
	};
}
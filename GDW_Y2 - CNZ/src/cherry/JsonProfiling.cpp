#include "JsonProfiling.h"
#include <toolkit/Logging.h>

cherry::ProfilingSession* cherry::ProfilingSession::__Session = nullptr;

void cherry::ProfilingSession::Start(const std::string& outPath)
{
	// If a session already exists, end it
	if (__Session != nullptr) {
		LOG_WARN("Ending profiling session!");
		delete __Session;
	}
	// Create a new session, and direct the output to the given path
	__Session = new ProfilingSession(outPath);
}

void cherry::ProfilingSession::End() {
	// Delete the session (closes the file), and set to null
	delete __Session;
	__Session = nullptr;
}

void cherry::ProfilingSession::WriteResult(const ProfilingResult& result)
{
	if (__Session != nullptr) {
		__Session->_WriteResult(result);
	} else {
		LOG_WARN("Failed to write result, session has been destroyed!");
	}
}

// Session destructor, writes the footer and closes the file
cherry::ProfilingSession::~ProfilingSession() {
	// We don't want exceptions to be thrown from the destructor, so we'll wrap it in a try-catch
	try {
		_WriteFooter();
		myOutStream.close();
	} catch(...) {
		LOG_ERROR("An error has occured shutting down the profiling session!");
	}
}

cherry::ProfilingSession::ProfilingSession(const std::string& outPath) {
	myOutStream.open(outPath);
	_WriteHeader();
	myProfileCount = 0l;
}

void cherry::ProfilingSession::_WriteHeader()
{
	myOutStream << R"L({"otherData": {}, "traceEvents": [)L";
	myOutStream.flush();
}

void cherry::ProfilingSession::_WriteFooter() {
	myOutStream << R"L(]})L";
	myOutStream.flush();
}

void cherry::ProfilingSession::_WriteResult(const ProfilingResult& result)
{
	// If we are not the first profile, we need to comma-seperate us!
	if (myProfileCount++ > 0)
		myOutStream << ",";

	// Make sure our name does not have any characters that will break our JSON
	std::string name = result.Name;
	std::replace(name.begin(), name.end(), '"', '\''); // Replace " with '
	
	myOutStream << "{";
	myOutStream << R"L("cat": "function",)L"; // Our category is always function for now
	myOutStream << R"L("dur": )L" << (result.End - result.Begin) << ","; // Duration
	myOutStream << R"L("name": ")L" << name << "\","; // Name of profiling function
	if (!result.Args.empty()) {
		std::string args = result.Args;
		std::replace(args.begin(), args.end(), '"', '\''); // Replace " with '
		myOutStream << R"L("args": {)L" << result.Args << "},"; // Name of profiling function
	}
	myOutStream << R"L("ph": "X",)L"; // By default we output complete phases (X)
	myOutStream << R"L("ts": )L" << result.Begin << ","; // TS is the timestep in microseconds
	myOutStream << R"L("tid": )L" << result.ThreadID << ","; // tid is the calling thread ID
	myOutStream << R"L("pid": 0)L"; // pid is the calling process ID (we'll hard code it to 0 for now)
	myOutStream << "}";

	// We flush our output after each result in case the app crashes, we don't lose all profiling info!
	myOutStream.flush();
}

#pragma once
#include <filesystem>
#include <string>

enum LogLevel {
	LogNone			= 0,
	LogInfo			= 1 << 0x1,
	LogWarning		= 1 << 0x2,
	LogError		= 1 << 0x3,
	LogConsole		= 1 << 0x4,
	LogFlush		= 1 << 0x5,

	LogFile = LogInfo | LogWarning | LogError
};

namespace Logger
{
	// LoggerManager starts paused so that printout can be started after all sub-loggers are initialized
	void Play();
	// Log to LoggerManager
	void LogToManager(const std::string& message, LogLevel level);
	// Add a new destination to LoggerManager
	void AddDestinations(const std::filesystem::path& log, const std::string& prefix, UInt32 logLevel);
	// Prepare for copying file
	void PrepareCopy(const std::filesystem::path& in, const std::filesystem::path& out);
	// Copy all prepared files
	void Copy();
}

class Log {
public:
	inline Log(UInt32 level = LogLevel::LogWarning) : logLevel((LogLevel)level) {};

	template <typename T>
	inline Log& operator<<(const T &value) {
		buffer << value;
		return *this;
	}

	inline Log& operator<<(std::ostream &(*func)(std::ostream &)) {
		buffer << func;
		return *this;
	}

	inline ~Log() { Logger::LogToManager(buffer.str(), logLevel); };

	bool empty() { return buffer.tellp() == 0; }

private:
	LogLevel logLevel;
	std::stringstream buffer;
};
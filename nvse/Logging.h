#pragma once
#include <filesystem>
#include <string>

enum LogLevel {
	None		= 0,
	Info		= 1 << 0x1,
	Warning		= 1 << 0x2,
	Error		= 1 << 0x3,
	Console		= 1 << 0x4,

	File = Info | Warning | Error
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
	inline Log(UInt32 level = LogLevel::Warning) : logLevel((LogLevel)level) {};

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
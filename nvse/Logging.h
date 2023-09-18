#pragma once
#include <filesystem>
#include <string>

enum class LogLevel {
	Info, Warning, Error
};

static std::string convertLevel(LogLevel level) {
	switch (level) {
	case LogLevel::Info:
		return "Info";
	case LogLevel::Warning:
		return "Warning";
	case LogLevel::Error:
		return "Error";
	default:
		return "";
	}
}

namespace Logger
{
	// LoggerManager starts paused so that printout can be started after all sub-loggers are initialized
	void Play();
	// Log to LoggerManager
	void LogToManager(const std::string& message, LogLevel level);
	// Add a new destination to LoggerManager
	void AddDestinations(const std::filesystem::path& log, LogLevel logLevel);
	// Prepare for copying file
	void PrepareCopy(const std::filesystem::path& in, const std::filesystem::path& out);
	// Copy all prepared files
	void Copy();
}

class Log {
public:
	inline Log() : logLevel(LogLevel::Error) {}
	inline Log(LogLevel level) : logLevel(level) {}
	inline Log(UInt32 one, UInt32 two) : logLevel(static_cast<LogLevel>(two)) {}

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

private:
	LogLevel logLevel;
	std::stringstream buffer;
};
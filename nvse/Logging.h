#pragma once
#include <filesystem>
#include <string>

class Log
{
	UInt32 logLevel;
public:

	enum
	{
		kToNone = 0,
		kToLog = 1,
		kToConsole = 2,
		kToBoth = kToLog | kToConsole
	};

	Log(UInt32 logLevel = 0) : logLevel(logLevel) {};
	Log& operator<<(const std::string str);

	static void Init(const std::filesystem::path& path, const std::string& modName);
};
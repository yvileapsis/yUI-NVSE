#pragma once
#include <filesystem>
#include <string>

class Log
{
	UInt32 logLevel;
public:

	enum
	{
		kNone = 0,
		kLog = 1,
		kConsole = 2,
		kBoth = kLog | kConsole
	};

	Log(UInt32 logLevel = 0) : logLevel(logLevel) {};
	Log& operator<<(const std::string& str);

	static void Init(const std::filesystem::path& path, const std::string& modName);
};
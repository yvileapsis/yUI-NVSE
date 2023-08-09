#pragma once
#include <filesystem>
#include <string>

class Log
{
	UInt32 logDest;
	UInt32 logLevel;
public:

	enum
	{
		kError = 1,
		kWarning = 2,
		kMessage = 3
	};

	enum
	{
		kNone = 0,
		kLog = 1,
		kConsole = 2,
		kBoth = kLog | kConsole
	};

	Log(UInt32 logLevel = true, UInt32 logDest = kLog) : logDest(logDest), logLevel(logLevel) {};
	Log& operator<<(const std::string& str);
	Log& operator>>(const std::filesystem::path& path);
	Log& operator()();

	static void Init(const std::filesystem::path& path, const std::string& modName);
	static void Copy(const std::filesystem::path& path);
};

void Dump(Tile* tile);
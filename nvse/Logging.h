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

	Log(UInt32 logLevel = kError, UInt32 logDest = kLog) : logDest(logDest), logLevel(logLevel) {};
	Log& operator<<(const std::string& str);

	static void Init(const std::filesystem::path& path, const std::string& modName);
};

void Dump(Tile* tile);
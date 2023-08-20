#pragma once
#include <filesystem>
#include <string>
#include <functional>

class Record;
class Log;

std::string FormatString(const char* fmt, ...);

typedef std::function<std::string()> InternalFunction;

class Record : public InternalFunction
{
public:
	template<typename... Args> Record(const char* str, Args&&... args)
	: InternalFunction(std::bind_front(FormatString, str, std::forward<Args>(args)...))
//	: InternalFunction([&, str]() { return FormatString(str, std::forward<Args>(args)...); })
	{}

	Record(std::string str) : InternalFunction([=]() { return str; }) {};

	//	friend Log& operator<<(const Log& log, Record rec);

	friend Log;
};

class Log
{
public:
	UInt32 logDest;
	UInt32 logLevel;

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
//	Log& operator<<(const std::string& str);
	Log& operator>>(const std::filesystem::path& path);
	Log& operator()();

	Log& operator<<(const Record& rec);


	static void Init(const std::filesystem::path& path, const std::string& modName);
	static void Copy(const std::filesystem::path& path);

	friend Record;
};

void Dump(Tile* tile);
#pragma once
#include <filesystem>
#include <string>

enum 
{
	kToNone = 0,
	kToLog = 1,
	kToConsole = 2,
	kToBoth = kToLog | kToConsole
};

void Log(const std::string& msg, UInt32 loglevel = 0);
void LogInit(const std::filesystem::path& path, const std::string& modName);
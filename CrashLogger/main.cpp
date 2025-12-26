#include <main.hpp>
#include "namegen.hpp"
#include <format>
#include <iostream>

IDebugLog	   gLog(CrashLogger_LOG);

void FillPluginInfo(PluginInfo* info)
{
	info->infoVersion = PluginInfo::kInfoVersion;
	info->name = "CrashLogger";
	info->version = CrashLogger_VERSION * 100;
}

void InitLog()
{
	{
		char logPath[MAX_PATH];
		GetCurPath(logPath, sizeof(logPath));
		strcat_s(logPath, CrashLogger_LOG);

		char logFolderPath[MAX_PATH];
		GetCurPath(logFolderPath, sizeof(logFolderPath));
		strcat_s(logFolderPath, CrashLogger_FLD);

		Logger::PrepareCopy(logPath, logFolderPath);
	}

	time_t now = time(nullptr);
	tm localTime;
	localtime_s(&localTime, &now);

	char nameBuffer[512];
	GetName(nameBuffer, sizeof(nameBuffer));

	_MESSAGE("%s version %s at %04d-%02d-%02d %02d:%02d:%02d\n"
		"If this file is empty, then your game didn't crash or something went so wrong even crash logger was useless! :snig:\n"
		"Topmost stack module is NOT ALWAYS the crash reason! Exercise caution when speculating!\n",
		nameBuffer,
		CrashLogger_VERSION_STR,
		localTime.tm_year + 1900,
		localTime.tm_mon + 1,
		localTime.tm_mday,
		localTime.tm_hour,
		localTime.tm_min,
		localTime.tm_sec);

	_MESSAGE("When asking for help, please send the whole log file!\n");

	if (GetModuleHandle("nvac.dll")) {
		_MESSAGE("NVAC detected! The log will be incorrect!\nPlease remove NVAC to let the game crash properly!\n");
	}


	if (GetModuleHandle("nvsr.dll")) {
		_MESSAGE("New Vegas Stutter Remover detected! Using it will cause crashes!\n");
	}
}

bool NVSEPlugin_Query(const NVSEInterface* nvse, PluginInfo* info)
{
	FillPluginInfo(info);

	// version checks
	if (nvse->isEditor) {
		return false;
	} else {
		if (nvse->version < PACKED_NVSE_VERSION) {
			_MESSAGE("NVSE version too old (got %08X expected at least %08X). Plugin will NOT load! Install the latest version here: https://github.com/xNVSE/NVSE/releases/", nvse->version, PACKED_NVSE_VERSION);
			return false;
		}

		if (nvse->isNogore) {
			_MESSAGE("NoGore is not supported");
			return false;
		}
	}

	return true;
}

bool NVSEPlugin_Preload() {
	Inits();

	InitLog();

	return true;
}

bool NVSEPlugin_Load(const NVSEInterface* nvse)
{
	if (nvse->isEditor)	return true;

	g_commandInterface = static_cast<NVSECommandTableInterface*>(nvse->QueryInterface(NVSEInterface::kInterface_CommandTable));

	return true;
}
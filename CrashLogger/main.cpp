#include <main.h>
#include <TESObjectREFR.h>
#include <GameData.h>
#include <Menu.h>

#include "CrashLogger.h"
#include "namegen.h"
#include <format>
#include <iostream>

void InitSingletons()
{
	g_player = PlayerCharacter::GetSingleton();
	g_TESDataHandler = TESDataHandler::GetSingleton();
	g_HUDMainMenu = HUDMainMenu::GetSingleton();
}

void FillPluginInfo(PluginInfo* info)
{
	info->infoVersion = PluginInfo::kInfoVersion;
	info->name = "CrashLogger";
	info->version = CrashLogger_VERSION * 100;
}

void InitLog(std::filesystem::path path = "")
{
	std::filesystem::path logPath = GetCurPath();
	logPath /= path;
	logPath /= CrashLogger_LOG;

	std::filesystem::path logFolderPath = GetCurPath();
	logFolderPath /= path;
	logFolderPath /= CrashLogger_FLD;
	logFolderPath /= CrashLogger_LOG;

	Logger::AddDestinations(logPath, CrashLogger_STR, LogLevel::File);
	Logger::PrepareCopy(logPath, logFolderPath);

	Log(LogLevel::Console) << CrashLogger_VERSION_STR;

	Log(LogLevel::Warning) << GetName() + " version " + CrashLogger_VERSION_STR + " at " + std::format("{0:%F} {0:%T}", std::chrono::time_point(std::chrono::system_clock::now())) << std::endl 
		<< "If this file is empty, then your game didn't crash or something went so wrong even crash logger was useless! :snig:" << std::endl
		<< "Topmost stack module is NOT ALWAYS the crash reason! Exercise caution when speculating!" << std::endl;
}

void MessageHandler(NVSEMessagingInterface::Message* msg)
{
	if (msg->type == NVSEMessagingInterface::kMessage_DeferredInit)
	{
		InitSingletons();
		Logger::Play();

		for (const auto& i : deferredInit) i(); // call all deferred init functions
	}
	else if (msg->type == NVSEMessagingInterface::kMessage_MainGameLoop)
	{
		if (!iMainLoopDoOnce && !MenuMode()) {
			iMainLoopDoOnce++;
			for (const auto& i : mainLoopDoOnce) i(); // call all do once functions
		}
		for (const auto& i : mainLoop) i(); // call all mainloop functions
	}
}

bool NVSEPlugin_Query(const NVSEInterface* nvse, PluginInfo* info)
{
	FillPluginInfo(info);
	// version checks
	if (nvse->isEditor) {
		if (nvse->editorVersion < CS_VERSION_1_4_0_518)
		{
			Log() << std::format("Incorrect editor version (got {:08X} need at least {:08X})", nvse->editorVersion, CS_VERSION_1_4_0_518);
			return false;
		}
	} else {
//		if (nvse->nvseVersion < PACKED_NVSE_VERSION) {
//			Log() << FormatString("NVSE version too old (got %X expected at least %X). Plugin will NOT load! Install the latest version here: https://github.com/xNVSE/NVSE/releases/", nvse->nvseVersion, PACKED_NVSE_VERSION);
//			return false;
//		}

		if (nvse->runtimeVersion < RUNTIME_VERSION_1_4_0_525) {
			Log() << std::format("Incorrect runtime version (got {:08X} need at least {:08X})", nvse->runtimeVersion, RUNTIME_VERSION_1_4_0_525);
			return false;
		}

		if (nvse->isNogore) {
			Log() << "NoGore is not supported";
			return false;
		}

	}

	CrashLogger::VirtualTables::FillNVSELabels();
	CrashLogger::ApplyNVSE();

	return true;
}

bool NVSEPlugin_Load(const NVSEInterface* nvse)
{
	g_pluginHandle = nvse->GetPluginHandle();
	g_nvseInterface = const_cast<NVSEInterface*>(nvse);
	g_messagingInterface = static_cast<NVSEMessagingInterface*>(nvse->QueryInterface(kInterface_Messaging));
	g_messagingInterface->RegisterListener(g_pluginHandle, "NVSE", MessageHandler);

	Inits();

	if (nvse->isEditor)	return true;

	g_stringInterface = static_cast<NVSEStringVarInterface*>(nvse->QueryInterface(kInterface_StringVar));
	GetStringVar = g_stringInterface->GetString;
	SetStringVar = g_stringInterface->SetString;
	AssignString = g_stringInterface->Assign;
	
	g_arrayInterface = static_cast<NVSEArrayVarInterface*>(nvse->QueryInterface(kInterface_ArrayVar));
	g_dataInterface = static_cast<NVSEDataInterface*>(nvse->QueryInterface(kInterface_Data));
	HasScriptCommand = reinterpret_cast<_HasScriptCommand>(g_dataInterface->GetFunc(NVSEDataInterface::kNVSEData_HasScriptCommand));
	DecompileScript = reinterpret_cast<_DecompileScript>(g_dataInterface->GetFunc(NVSEDataInterface::kNVSEData_DecompileScript));

	g_DIHook = static_cast<DIHookControl*>(g_dataInterface->GetSingleton(NVSEDataInterface::kNVSEData_DIHookControl));

	g_commandInterface = static_cast<NVSECommandTableInterface*>(nvse->QueryInterface(kInterface_CommandTable));
	GetByOpcode = g_commandInterface->GetByOpcode;

	g_scriptInterface = static_cast<NVSEScriptInterface*>(nvse->QueryInterface(kInterface_Script));
	ExtractArgsEx = g_scriptInterface->ExtractArgsEx;
	ExtractFormatStringArgs = g_scriptInterface->ExtractFormatStringArgs;
	CallFunctionAlt = g_scriptInterface->CallFunctionAlt;
	CompileExpression = g_scriptInterface->CompileExpression;
	CompileScript = g_scriptInterface->CompileScript;

	g_eventInterface = static_cast<NVSEEventManagerInterface*>(nvse->QueryInterface(kInterface_EventManager));
	SetNativeEventHandler = g_eventInterface->SetNativeEventHandler;
	RemoveNativeEventHandler = g_eventInterface->RemoveNativeEventHandler;
	SetHandlerFunctionValue = g_eventInterface->SetNativeHandlerFunctionValue;
	RegisterEvent = g_eventInterface->RegisterEvent;
	DispatchEvent = g_eventInterface->DispatchEvent;

	g_loggingInterface = static_cast<NVSELoggingInterface*>(nvse->QueryInterface(kInterface_LoggingInterface));

	InitLog(g_loggingInterface ? g_loggingInterface->GetPluginLogPath() : "");

	for (const auto& i : pluginLoad) i(); // call all plugin load functions

	return true;
}

bool FOSEPlugin_Query(const OBSEInterface* obse, PluginInfo* info)
{
	FillPluginInfo(info);

	if (obse->isEditor) {
	}
	else {
		if (obse->obseVersion < 1) {
			Log() << std::format("FOSE version too old (got {:08X}; expected at least {:08X}).", obse->obseVersion, 1);
			return false;
		}
		if (obse->oblivionVersion != 0x01070030) {
			Log() << std::format("incorrect Fallout 3 version (got {:08X}; need {:08X}).", obse->oblivionVersion, 0x01070030);
			return false;
		}
	}

	CrashLogger::VirtualTables::FillFOSELabels();
	CrashLogger::ApplyFOSE();

	return true;
}

bool FOSEPlugin_Load(const OBSEInterface* obse)
{
	g_pluginHandle = obse->GetPluginHandle();

	Inits();

	if (obse->isEditor) return true;

	InitLog();
	for (const auto& i : pluginLoad) i(); // call all plugin load functions

	return true;
}

bool OBSEPlugin_Query(const OBSEInterface* obse, PluginInfo* info)
{
	FillPluginInfo(info);

	if (obse->isEditor) {
	}
	else {
		if (obse->obseVersion < 21) {
			Log() << std::format("OBSE version too old (got {:08X}; expected at least {:08X}).", obse->obseVersion, 21);
			return false;
		}
		if (obse->oblivionVersion != OBLIVION_VERSION_1_2_416) {
			Log() << std::format("incorrect Oblivion version (got {:08X}; need {:08X}).", obse->oblivionVersion, OBLIVION_VERSION_1_2_416);
			return false;
		}
	}

	CrashLogger::VirtualTables::FillOBSELabels();
	CrashLogger::ApplyOBSE();

	return true;
}

bool OBSEPlugin_Load(const OBSEInterface* obse) {
	g_pluginHandle = obse->GetPluginHandle();

	Inits();

	if (obse->isEditor) return true;

	InitLog();
	for (const auto& i : pluginLoad) i(); // call all plugin load functions

	return true;
}

#include <main.h>

void InitSingletons()
{
	g_player = PlayerCharacter::GetSingleton();
	g_TESDataHandler = TESDataHandler::GetSingleton();
	g_HUDMainMenu = HUDMainMenu::GetSingleton();
}

void InitLog(const std::filesystem::path& path = "")
{
	std::filesystem::path logPath = GetCurPath();
	logPath /= path;
	logPath /= yUI_LOG;

	Logger::AddDestinations(logPath, yUI_STR, LogLevel::LogWarning | LogLevel::LogError);

	Log(LogLevel::LogConsole) << yUI_VERSION_STR;
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
		if (!bMainLoopDoOnce && !MenuMode()) {
			bMainLoopDoOnce = true;
			for (const auto& i : mainLoopDoOnce) i(); // call all do once functions
		}
		for (const auto& i : mainLoop) i(); // call all mainloop functions

	}
}

bool NVSEPlugin_Query(const NVSEInterface* nvse, PluginInfo* info)
{
	// fill out the info structure
	info->infoVersion = PluginInfo::kInfoVersion;
	info->name = "yUI";
	info->version = yUI_VERSION * 100;

	// version checks
	if (nvse->isEditor) {
		if (nvse->editorVersion < CS_VERSION_1_4_0_518)
		{
			Log() << std::format("yUI: incorrect editor version (got {:08X} need at least {:08X})", nvse->editorVersion, CS_VERSION_1_4_0_518);
			return false;
		}
	} else {
		if (nvse->version < PACKED_NVSE_VERSION) {
			Log() << std::format("yUI: NVSE version too old (got {:X} expected at least {:X}). Plugin will NOT load! Install the latest version here: https://github.com/xNVSE/NVSE/releases/", nvse->version, PACKED_NVSE_VERSION);
			return false;
		}

		if (nvse->gameVersion < RUNTIME_VERSION_1_4_0_525) {
			Log() << std::format("yUI: incorrect runtime version (got {:08X} need at least {:08X})", nvse->gameVersion, RUNTIME_VERSION_1_4_0_525);
			return false;
		}

		if (nvse->isNogore) {
			Log() << "yUI: NoGore is not supported";
			return false;
		}
	}
	return true;
}

bool NVSEPlugin_Load(const NVSEInterface* nvse)
{
	g_pluginHandle = nvse->GetPluginHandle();
	g_nvseInterface = const_cast<NVSEInterface*>(nvse);
	g_messagingInterface = static_cast<NVSEMessagingInterface*>(nvse->QueryInterface(NVSEInterface::kInterface_Messaging));
	g_messagingInterface->RegisterListener(g_pluginHandle, "NVSE", MessageHandler);

	Inits();

	if (nvse->isEditor)	return true;

	g_stringInterface = static_cast<NVSEStringVarInterface*>(nvse->QueryInterface(NVSEInterface::kInterface_StringVar));
	GetStringVar = g_stringInterface->GetString;
	SetStringVar = g_stringInterface->SetString;
	AssignString = g_stringInterface->Assign;
	
	g_arrayInterface = static_cast<NVSEArrayVarInterface*>(nvse->QueryInterface(NVSEInterface::kInterface_ArrayVar));
	g_dataInterface = static_cast<NVSEDataInterface*>(nvse->QueryInterface(NVSEInterface::kInterface_Data));
	HasScriptCommand = reinterpret_cast<_HasScriptCommand>(g_dataInterface->GetFunc(NVSEDataInterface::kNVSEData_HasScriptCommand));

	g_DIHook = static_cast<DIHookControl*>(g_dataInterface->GetSingleton(NVSEDataInterface::kNVSEData_DIHookControl));

	g_commandInterface = static_cast<NVSECommandTableInterface*>(nvse->QueryInterface(NVSEInterface::kInterface_CommandTable));
	GetByOpcode = g_commandInterface->GetByOpcode;

	g_scriptInterface = static_cast<NVSEScriptInterface*>(nvse->QueryInterface(NVSEInterface::kInterface_Script));
	ExtractArgsEx = g_scriptInterface->ExtractArgsEx;
	ExtractFormatStringArgs = g_scriptInterface->ExtractFormatStringArgs;
	CallFunctionAlt = g_scriptInterface->CallFunctionAlt;
	CompileExpression = g_scriptInterface->CompileExpression;
	CompileScript = g_scriptInterface->CompileScript;

	g_eventInterface = static_cast<NVSEEventManagerInterface*>(nvse->QueryInterface(NVSEInterface::kInterface_EventManager));
	SetNativeEventHandler = g_eventInterface->SetNativeEventHandler;
	RemoveNativeEventHandler = g_eventInterface->RemoveNativeEventHandler;
	SetHandlerFunctionValue = g_eventInterface->SetNativeHandlerFunctionValue;
	RegisterEvent = g_eventInterface->RegisterEvent;
	DispatchEvent = g_eventInterface->DispatchEvent;

	g_loggingInterface = static_cast<NVSELoggingInterface*>(nvse->QueryInterface(NVSEInterface::kInterface_LoggingInterface));

	InitLog(g_loggingInterface ? g_loggingInterface->GetPluginLogPath() : "");

	for (const auto& i : pluginLoad) i(); // call all plugin load functions

	return true;
}

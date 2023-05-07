#include <main.h>
#include <TESObjectREFR.h>
#include <GameData.h>
#include <Menu.h>

void InitSingletons()
{
	g_player = PlayerCharacter::GetSingleton();
	g_TESDataHandler = TESDataHandler::GetSingleton();
	g_HUDMainMenu = HUDMainMenu::GetSingleton();
}

void MessageHandler(NVSEMessagingInterface::Message* msg)
{
	if (msg->type == NVSEMessagingInterface::kMessage_DeferredInit)
	{
		InitSingletons();

		Log(true, Log::kBoth) << yUI_VERSION_STR;

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
	// fill out the info structure
	info->infoVersion = PluginInfo::kInfoVersion;
	info->name = "yUI";
	info->version = yUI_VERSION * 100;

	// version checks
	if (nvse->isEditor) {
		if (nvse->editorVersion < CS_VERSION_1_4_0_518)
		{
			Log(true, Log::kLog) << FormatString("yUI: incorrect editor version (got %08X need at least %08X)", nvse->editorVersion, CS_VERSION_1_4_0_518);
			return false;
		}
	} else {
		if (nvse->nvseVersion < PACKED_NVSE_VERSION) {
			Log(true, Log::kLog) << FormatString("yUI: NVSE version too old (got %X expected at least %X). Plugin will NOT load! Install the latest version here: https://github.com/xNVSE/NVSE/releases/", nvse->nvseVersion, PACKED_NVSE_VERSION);
			return false;
		}

		if (nvse->runtimeVersion < RUNTIME_VERSION_1_4_0_525) {
			Log(true, Log::kLog) << FormatString("yUI: incorrect runtime version (got %08X need at least %08X)", nvse->runtimeVersion, RUNTIME_VERSION_1_4_0_525);
			return false;
		}

		if (nvse->isNogore) {
			Log(true, Log::kLog) << FormatString("yUI: NoGore is not supported");
			return false;
		}
	}
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
	auto path = std::filesystem::path(g_loggingInterface->GetPluginLogPath());
	path += yUI_LOG;
	Log::Init(path, yUI_STR);
	
	for (const auto& i : pluginLoad) i(); // call all plugin load functions

	return true;
}

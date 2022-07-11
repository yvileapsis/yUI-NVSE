#include <main.h>
#include <functions.h>
#include <patches.h>
#include <settings.h>
#include <file.h>

#include <JDC.h>

#define yJAM_VERSION 0.1
#define yJAM_VERSION_STR "0.1"
#define yJAM_STR "yJAM"

PluginHandle	g_pluginHandle = kPluginHandle_Invalid;

void MessageHandler(NVSEMessagingInterface::Message* msg)
{
	if (msg->type == NVSEMessagingInterface::kMessage_DeferredInit)
	{
		g_player = PlayerCharacter::GetSingleton();
		g_dataHandler = DataHandler::GetSingleton();
		Log(FormatString("%s", yJAM_VERSION_STR), 2);

		PrintAndClearQueuedConsoleMessages();

	}
	else if (msg->type == NVSEMessagingInterface::kMessage_MainGameLoop)
	{
		if (g_JDC) JDC::MainLoop();
		if (iDoOnce == 0 && !CdeclCall<bool>(0x702360)) {
			iDoOnce++;

			if (g_JDC) JDC::Initialize();

			SetNativeEventHandler("JDCReset", reinterpret_cast<EventHandler>(JDC::Wraaa));
//			SetNativeEventHandler("OnDrop", reinterpret_cast<EventHandler>(SI::TestFunc));
		}
		
	}
}

bool NVSEPlugin_Query(const NVSEInterface* nvse, PluginInfo* info)
{
	gLog.Create("yJAM.log");
	gLog.SetModString(yJAM_STR);
	Log("Query", 1);

	// fill out the info structure
	info->infoVersion = PluginInfo::kInfoVersion;
	info->name = "yJAM";
	info->version = yJAM_VERSION * 100;

	// version checks
	if (nvse->isEditor) {
		if (nvse->editorVersion < CS_VERSION_1_4_0_518)
		{
			Log(FormatString("yJAM: incorrect editor version (got %08X need at least %08X)", nvse->editorVersion, CS_VERSION_1_4_0_518), 1);
			return false;
		}
	} else {
		if (nvse->nvseVersion < PACKED_NVSE_VERSION) {
			Log(FormatString("yJAM: NVSE version too old (got %X expected at least %X). Plugin will NOT load! Install the latest version here: https://github.com/xNVSE/NVSE/releases/", nvse->nvseVersion, PACKED_NVSE_VERSION), 1);
			return false;
		}

		if (nvse->runtimeVersion < RUNTIME_VERSION_1_4_0_525) {
			Log(FormatString("yJAM: incorrect runtime version (got %08X need at least %08X)", nvse->runtimeVersion, RUNTIME_VERSION_1_4_0_525), 1);
			return false;
		}

		if (nvse->isNogore) {
			Log(FormatString("yJAM: NoGore is not supported"), 1);
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

	if (nvse->isEditor)	return true;

	g_stringInterface = static_cast<NVSEStringVarInterface*>(nvse->QueryInterface(kInterface_StringVar));
	GetStringVar = g_stringInterface->GetString;
	SetStringVar = g_stringInterface->SetString;
	AssignString = g_stringInterface->Assign;
	
	g_arrayInterface = static_cast<NVSEArrayVarInterface*>(nvse->QueryInterface(kInterface_ArrayVar));
	g_scriptInterface = static_cast<NVSEScriptInterface*>(nvse->QueryInterface(kInterface_Script));
	g_dataInterface = static_cast<NVSEDataInterface*>(nvse->QueryInterface(kInterface_Data));

	g_commandInterface = static_cast<NVSECommandTableInterface*>(nvse->QueryInterface(kInterface_CommandTable));

	ExtractArgsEx = g_scriptInterface->ExtractArgsEx;

	g_eventInterface = static_cast<NVSEEventManagerInterface*>(nvse->QueryInterface(kInterface_EventManager));

	SetNativeEventHandler = g_eventInterface->SetNativeEventHandler;
	RemoveNativeEventHandler = g_eventInterface->RemoveNativeEventHandler;

	handleINIOptions();

	return true;
}

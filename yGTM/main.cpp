#include <main.h>
#include <GameObjects.h>
#include <commands.h>
#include <patches.h>
#include <settings.h>
#include <timeMult.h>

#define yGTM_VERSION 3.0
#define yGTM_VERSION_STR "3.0"

PluginHandle	g_pluginHandle = kPluginHandle_Invalid;

void MessageHandler(NVSEMessagingInterface::Message* msg)
{
	if (msg->type == NVSEMessagingInterface::kMessage_DeferredInit)
	{
		g_player = PlayerCharacter::GetSingleton();
		g_dataHandler = DataHandler::GetSingleton();

		PrintAndClearQueuedConsoleMessages();
		TimeMult::FillMaps();

	}
	else if (msg->type == NVSEMessagingInterface::kMessage_MainGameLoop)
	{
		if (iDoOnce == 0 && !CdeclCall<bool>(0x702360)) {
			iDoOnce++;
		}
	}
}

bool NVSEPlugin_Query(const NVSEInterface* nvse, PluginInfo* info)
{
	gLog.Create("yGTM.log");
	PrintLog("yGTM: Query");

	info->infoVersion = PluginInfo::kInfoVersion;
	info->name = "yGTM";
	info->version = yGTM_VERSION;

	if (nvse->isEditor) {
		if (nvse->editorVersion < CS_VERSION_1_4_0_518)
		{
			PrintLog("yGTM: incorrect editor version (got %08X need at least %08X)", nvse->editorVersion, CS_VERSION_1_4_0_518);
			return false;
		}
	}
	else {
		if (nvse->nvseVersion < PACKED_NVSE_VERSION) {
			PrintLog("yGTM: NVSE version too old (got %X expected at least %X). Plugin will NOT load! Install the latest version here: https://github.com/xNVSE/NVSE/releases/", nvse->nvseVersion, PACKED_NVSE_VERSION);
			return false;
		}

		if (nvse->runtimeVersion < RUNTIME_VERSION_1_4_0_525) {
			PrintLog("yGTM: incorrect runtime version (got %08X need at least %08X)", nvse->runtimeVersion, RUNTIME_VERSION_1_4_0_525);
			return false;
		}

		if (nvse->isNogore) {
			PrintLog("yGTM: NoGore is not supported");
			return false;
		}
	}
	return true;
}

void writePatch()
{
	if (g_yTM) patchTimeMult();
}

bool NVSEPlugin_Load(const NVSEInterface* nvse)
{
	g_pluginHandle = nvse->GetPluginHandle();
	g_nvseInterface = const_cast<NVSEInterface*>(nvse);
	g_messagingInterface = static_cast<NVSEMessagingInterface*>(nvse->QueryInterface(kInterface_Messaging));
	g_messagingInterface->RegisterListener(g_pluginHandle, "NVSE", MessageHandler);

	g_nvseInterface = const_cast<NVSEInterface*>(nvse);

	const auto cmdTable = static_cast<NVSECommandTableInterface*>(nvse->QueryInterface(kInterface_CommandTable));

	GetByOpcode = cmdTable->GetByOpcode;

	GetByOpcode(0x22B0)->numParams = 1;
	GetByOpcode(0x22B0)->params = kParams_OneOptionalInt;

	GetByOpcode(0x1186)->numParams = 2;
	GetByOpcode(0x1186)->params = kParams_OneOptionalFloat_OneOptionalInt;
	
	if (nvse->isEditor)	return true;

	g_dataInterface = static_cast<NVSEDataInterface*>(nvse->QueryInterface(kInterface_Data));
	HasScriptCommand = static_cast<_HasScriptCommand>(g_dataInterface->GetFunc(NVSEDataInterface::kNVSEData_HasScriptCommand));
	
	g_scriptInterface = static_cast<NVSEScriptInterface*>(nvse->QueryInterface(kInterface_Script));
	ExtractArgsEx = g_scriptInterface->ExtractArgsEx;
	
	GetByOpcode(0x22B0)->execute = Cmd_GetGlobalTimeMultiplierAlt_Execute;
	GetByOpcode(0x1186)->execute = Cmd_SetGlobalTimeMultiplierAlt_Execute;

	writePatch();

	return true;
}
#include <main.h>
#include <GameObjects.h>
#include <commands.h>
#include <patches.h>
#include <settings.h>
#include <timeMult.h>

#define yGTM_VERSION 3.0
#define yGTM_VERSION_STR "3.0"
#define yGTM_STR "yGTM"

PluginHandle	g_pluginHandle = kPluginHandle_Invalid;

void MessageHandler(NVSEMessagingInterface::Message* msg)
{
	if (msg->type == NVSEMessagingInterface::kMessage_DeferredInit)
	{
		g_player = PlayerCharacter::GetSingleton();
		g_dataHandler = DataHandler::GetSingleton();
		Log(FormatString("%s", yGTM_VERSION_STR), 1);

		PrintAndClearQueuedConsoleMessages();
		TimeMult::FillMaps();

	} else if (msg->type == NVSEMessagingInterface::kMessage_MainGameLoop) {
		if (iDoOnce == 0 && !CdeclCall<bool>(0x702360)) {
			iDoOnce++;
		}
	}
}

bool NVSEPlugin_Query(const NVSEInterface* nvse, PluginInfo* info)
{
	gLog.Create("yGTM.log");
	gLog.SetModString(yGTM_STR);
	Log("Query", 1);

	info->infoVersion = PluginInfo::kInfoVersion;
	info->name = "yGTM";
	info->version = yGTM_VERSION * 100;

	if (nvse->isEditor) {
		if (nvse->editorVersion < CS_VERSION_1_4_0_518)
		{
			Log(FormatString("yGTM: incorrect editor version (got %08X need at least %08X)", nvse->editorVersion, CS_VERSION_1_4_0_518));
			return false;
		}
	}
	else {
		if (nvse->nvseVersion < PACKED_NVSE_VERSION) {
			Log(FormatString("yGTM: NVSE version too old (got %X expected at least %X). Plugin will NOT load! Install the latest version here: https://github.com/xNVSE/NVSE/releases/", nvse->nvseVersion, PACKED_NVSE_VERSION));
			return false;
		}

		if (nvse->runtimeVersion < RUNTIME_VERSION_1_4_0_525) {
			Log(FormatString("yGTM: incorrect runtime version (got %08X need at least %08X)", nvse->runtimeVersion, RUNTIME_VERSION_1_4_0_525));
			return false;
		}

		if (nvse->isNogore) {
			Log(FormatString("yGTM: NoGore is not supported"));
			return false;
		}
	}
	return true;
}

void writePatch()
{
	patchTimeMult(g_yTM);
	patchFixExplosionPushForce(g_FixExplosionPushForce);
	patchRestoreSpreadGameSettings(g_RestoreSpreadGameSettings);
	patchCorrectAmmoEffects();
	patchCorrectMeltdownEffects();
	patchCorrectWeaponEffects();
//	patchMultiplicativeProjectileCount();
}
UInt32 g_exec;

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

	GetByOpcode(0x11E2)->numParams = 5;
	GetByOpcode(0x11E2)->params = kParams_OneForm_OneOptionalForm;

	if (nvse->isEditor)	return true;

	g_dataInterface = static_cast<NVSEDataInterface*>(nvse->QueryInterface(kInterface_Data));
	HasScriptCommand = static_cast<_HasScriptCommand>(g_dataInterface->GetFunc(NVSEDataInterface::kNVSEData_HasScriptCommand));
	
	g_scriptInterface = static_cast<NVSEScriptInterface*>(nvse->QueryInterface(kInterface_Script));
	ExtractArgsEx = g_scriptInterface->ExtractArgsEx;
	
	GetByOpcode(0x22B0)->execute = Cmd_GetGlobalTimeMultiplierAlt_Execute;
	GetByOpcode(0x1186)->execute = Cmd_SetGlobalTimeMultiplierAlt_Execute;
	GetByOpcode(0x11E2)->execute = Cmd_FireWeaponAlt_Execute;

	handleINIOptions();
	writePatch();

	return true;
}

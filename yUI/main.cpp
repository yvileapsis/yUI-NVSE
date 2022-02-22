#include <main.h>
#include <functions.h>
#include <patches.h>
#include <commands.h>
#include <settings.h>
#include <SortingIcons.h>
#include <ConfigurationMenu.h>
#include <file.h>

#define yUI_VERSION 1.4
#define yUI_VERSION_STR "1.4c"
#define yUI_STR "yUI"

PluginHandle	g_pluginHandle = kPluginHandle_Invalid;

void MessageHandler(NVSEMessagingInterface::Message* msg)
{
	if (msg->type == NVSEMessagingInterface::kMessage_DeferredInit)
	{
		g_player = PlayerCharacter::GetSingleton();
		g_dataHandler = DataHandler::GetSingleton();
		Log(FormatString("%s", yUI_VERSION_STR), 2);

		PrintAndClearQueuedConsoleMessages();
		FillCraftingComponents();

		if (g_ySI_Sort || g_ySI_Icons || g_ySI_Hotkeys || g_ySI_Categories) LoadSIMapsFromFiles();
	}
	else if (msg->type == NVSEMessagingInterface::kMessage_MainGameLoop)
	{
		if (g_FixTablineSelected) FixTablineSelected();
		if (g_ySI_Categories) SI::KeyringRefreshPostStewie();
		
		if (iDoOnce == 0 && !CdeclCall<bool>(0x702360)) {
			iDoOnce++;

			if (g_ySI_Icons) SI::InjectTemplates();
		}
		
	}
}

void writePatches()
{
	patchSortingHooks		(g_FixIndefiniteSorting || (g_ySI && g_ySI_Sort));
	patchFixDroppedItems	(g_FixDroppedItems);
	patchAddIcons			(g_ySI && g_ySI_Icons);
	patchReplaceHotkeyIcons	(g_ySI && g_ySI_Hotkeys);
	patchSortingCategories	(g_ySI && g_ySI_Categories);
	patchMatchedCursor		(g_yMC);
}

bool NVSEPlugin_Query(const NVSEInterface* nvse, PluginInfo* info)
{
	gLog.Create("yUI.log");
	gLog.SetModString(yUI_STR);
	Log("Query", 1);

	// fill out the info structure
	info->infoVersion = PluginInfo::kInfoVersion;
	info->name = "yUI";
	info->version = yUI_VERSION * 100;

	// version checks
	if (nvse->isEditor) {
		if (nvse->editorVersion < CS_VERSION_1_4_0_518)
		{
			Log(FormatString("yUI: incorrect editor version (got %08X need at least %08X)", nvse->editorVersion, CS_VERSION_1_4_0_518), 1);
			return false;
		}
	} else {
		if (nvse->nvseVersion < PACKED_NVSE_VERSION) {
			Log(FormatString("yUI: NVSE version too old (got %X expected at least %X). Plugin will NOT load! Install the latest version here: https://github.com/xNVSE/NVSE/releases/", nvse->nvseVersion, PACKED_NVSE_VERSION), 1);
			return false;
		}

		if (nvse->runtimeVersion < RUNTIME_VERSION_1_4_0_525) {
			Log(FormatString("yUI: incorrect runtime version (got %08X need at least %08X)", nvse->runtimeVersion, RUNTIME_VERSION_1_4_0_525), 1);
			return false;
		}

		if (nvse->isNogore) {
			Log(FormatString("yUI: NoGore is not supported"), 1);
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

	nvse->SetOpcodeBase(0x21D0);
	REG_CMD_STR(ySIGetTrait);
	REG_CMD(ySISetTrait);
//	REG_CMD(SwapTexatlas);

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
/*
	RegisterTraitID("&runsnig;", 2032);
	RegisterTraitID("runsnig", 2032);
	WriteRelJump(0xA095D1, reinterpret_cast<UInt32>(funpatch));
*/
//	writeMCMHooks();

//	CaptureLambdaVars = static_cast<_CaptureLambdaVars>(g_dataInterface->GetFunc(NVSEDataInterface::kNVSEData_LambdaSaveVariableList));
//	UncaptureLambdaVars = static_cast<_UncaptureLambdaVars>(g_dataInterface->GetFunc(NVSEDataInterface::kNVSEData_LambdaUnsaveVariableList));
	handleINIOptions();
	writePatches();

	return true;
}

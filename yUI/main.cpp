#include "main.h"
#include "PluginAPI.h"
#include "nvse/CommandTable.h"
//#include "commands_animation.h"
//#include "hooks.h"
#include "utility.h"

#include "GameData.h"
#include "GameRTTI.h"
#include "SafeWrite.h"
#include "game_types.h"

#include "LambdaVariableContext.h"
#include "nihooks.h"
#include "NiObjects.h"
#include "SimpleINILibrary.h"

#include "functions.h"

#include "patches.h"
#include "commands.h"
#include "settings.h"
#include "ySI.h"
#include <file.h>

#define yUI_VERSION 0.9

#define RegisterScriptCommand(name) 	nvse->RegisterCommand(&kCommandInfo_ ##name)
#define REG_CMD_STR(name) nvse->RegisterTypedCommand(&kCommandInfo_##name, kRetnType_String)

PluginHandle	g_pluginHandle = kPluginHandle_Invalid;

//const CommandInfo* g_TFC;
std::deque<std::function<void()>> g_executionQueue;

_CaptureLambdaVars CaptureLambdaVars;
_UncaptureLambdaVars UncaptureLambdaVars;

void HandleDebug()
{

}

void MessageHandler(NVSEMessagingInterface::Message* msg)
{
	if (msg->type == NVSEMessagingInterface::kMessage_DeferredInit)
	{
		g_player = PlayerCharacter::GetSingleton();
		g_dataHandler = DataHandler::GetSingleton();

		g_HUDMainMenu = Tile::GetMenuTile(kMenuType_HUDMain);
		g_StartMenu = Tile::GetMenuTile(kMenuType_Start);
		g_BarterMenu = Tile::GetMenuTile(kMenuType_Barter);
		g_ContainerMenu = Tile::GetMenuTile(kMenuType_Container);
		g_RepairMenu = Tile::GetMenuTile(kMenuType_Repair);
		g_RepairServicesMenu = Tile::GetMenuTile(kMenuType_RepairServices);
		g_MapMenu = Tile::GetMenuTile(kMenuType_Map);
		g_StatsMenu = Tile::GetMenuTile(kMenuType_Stats);
		g_allFormsMap = reinterpret_cast<NiTPointerMap<TESForm>**>(0x11C54C0);
		FillCraftingComponents();
		PrintAndClearQueuedConsoleMessages();

		if (g_ySI) LoadSIMapsFromFiles();

//		LoadFileAnimPaths();
		Console_Print("yUI %.2f", yUI_VERSION);
	}
	else if (msg->type == NVSEMessagingInterface::kMessage_MainGameLoop)
	{
		const auto isMenuMode = CdeclCall<bool>(0x702360);
		if (!isMenuMode)
		{

#if _DEBUG
			HandleDebug();
#endif
		}

		if (iDoOnce == 0) {
			if (!isMenuMode) {
				iDoOnce++;

//				ySIFillIconMap();

//				if (g_ySI) ySIFillTagMapAlt();
			}
		}
		
	}
	else if (msg->type == NVSEMessagingInterface::kMessage_PostLoadGame)
	{
//		for (auto* ctx : g_clearSounds)
//		{
//			ctx->soundPaths->items.clear();
//		}
//		g_clearSounds.clear();
//		g_partialLoopReloadState = PartialLoopingReloadState::NotSet;
//		g_reloadTracker.clear();
	}

}

void writePatches()
{
	if (g_ySI) patchRemoveTagsAddIcons();
	if (g_yCM) patchAddyCMToSettingsMenu();
	//	patch1080pUI();
}

bool NVSEPlugin_Query(const NVSEInterface* nvse, PluginInfo* info)
{
	_MESSAGE("query");

	// fill out the info structure
	info->infoVersion = PluginInfo::kInfoVersion;
	info->name = "yUI";
	info->version = yUI_VERSION;

	// version checks
	if (!nvse->isEditor) {
		if (nvse->nvseVersion < PACKED_NVSE_VERSION) {
			_ERROR("yUI: NVSE version too old (got %X expected at least %X). Plugin will NOT load! Install the latest version here: https://github.com/xNVSE/NVSE/releases/", nvse->nvseVersion, PACKED_NVSE_VERSION);
			return false;
		}

		if (nvse->runtimeVersion < RUNTIME_VERSION_1_4_0_525) {
			_ERROR("yUI: incorrect runtime version (got %08X need at least %08X)", nvse->runtimeVersion, RUNTIME_VERSION_1_4_0_525);
			return false;
		}

		if (nvse->isNogore) {
			_ERROR("yUI: NoGore is not supported");
			return false;
		}
	}

	else
	{
		if (nvse->editorVersion < CS_VERSION_1_4_0_518)
		{
			_ERROR("yUI: incorrect editor version (got %08X need at least %08X)", nvse->editorVersion, CS_VERSION_1_4_0_518);
			return false;
		}
	}
	return true;
}

int g_logLevel = 0;

bool NVSEPlugin_Load(const NVSEInterface* nvse)
{

	g_pluginHandle = nvse->GetPluginHandle();
	g_nvseInterface = const_cast<NVSEInterface*>(nvse);
	g_messagingInterface = static_cast<NVSEMessagingInterface*>(nvse->QueryInterface(kInterface_Messaging));
	g_messagingInterface->RegisterListener(g_pluginHandle, "NVSE", MessageHandler);

	nvse->SetOpcodeBase(0x21D0);
	REG_CMD_STR(ySIGetTagTrait);

	
	if (nvse->isEditor)	return true;


	g_stringInterface = static_cast<NVSEStringVarInterface*>(nvse->QueryInterface(kInterface_StringVar));
	GetStringVar = g_stringInterface->GetString;
	SetStringVar = g_stringInterface->SetString;
	AssignString = g_stringInterface->Assign;

	g_arrayInterface = static_cast<NVSEArrayVarInterface*>(nvse->QueryInterface(kInterface_ArrayVar));
	g_scriptInterface = static_cast<NVSEScriptInterface*>(nvse->QueryInterface(kInterface_Script));
	g_dataInterface = static_cast<NVSEDataInterface*>(nvse->QueryInterface(kInterface_Data));


//	CaptureLambdaVars = static_cast<_CaptureLambdaVars>(g_dataInterface->GetFunc(NVSEDataInterface::kNVSEData_LambdaSaveVariableList));
//	UncaptureLambdaVars = static_cast<_UncaptureLambdaVars>(g_dataInterface->GetFunc(NVSEDataInterface::kNVSEData_LambdaUnsaveVariableList));
	
//	ApplyHooks();
//	ApplyNiHooks();
	writePatches();
	return true;
}

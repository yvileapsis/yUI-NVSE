#pragma warning( disable : 4996 4800 4102 4100 4725 4201 4610 4510)
#include "NiPoint.h"
#include "CommandTable.h"
#include "main.h"
#include "ySI.h"
#include "Version.h"
#include "memory.h"
#include "errorlogging.h"
#include "patches.h"
#include "Combat.h"
#include "functions.h"

void MessageHandler(NVSEMessagingInterface::Message* msg)
{
	switch (msg->type)
	{
	case NVSEMessagingInterface::kMessage_PostLoadGame:
	{
		//		if (g_ySI) ySIFillMaps();
		//		iDoOnce++;
				// no break - run NewGame events too
	}
	case NVSEMessagingInterface::kMessage_NewGame:
	{
		//	shouldRefreshQuestMenu = true;
		//	lastQuest = nullptr;
		break;
	}

	case NVSEMessagingInterface::kMessage_SaveGame:
	{
		break;
	}
	case NVSEMessagingInterface::kMessage_PostPostLoad:
	{
		break;
	}
	case NVSEMessagingInterface::kMessage_DeferredInit:
	{
		g_thePlayer = PlayerCharacter::GetSingleton();
		g_dataHandler = DataHandler::GetSingleton();

		g_HUDMainMenu = GetMenuTile(kMenuType_HUDMain);
		g_StartMenu = GetMenuTile(kMenuType_Start);
		g_BarterMenu = GetMenuTile(kMenuType_Barter);
		g_ContainerMenu = GetMenuTile(kMenuType_Container);
		g_RepairMenu = GetMenuTile(kMenuType_Repair);
		g_RepairServicesMenu = GetMenuTile(kMenuType_RepairServices);
		g_MapMenu = GetMenuTile(kMenuType_Map);
		g_StatsMenu = GetMenuTile(kMenuType_Stats);
		FillCraftingComponent();

		PrintAndClearQueuedConsoleMessages();
		break;
	}
	case NVSEMessagingInterface::kMessage_MainGameLoop:
	{
		if (iDoOnce == 0) {
			if (GetIsMenuMode() == 0) {
				iDoOnce++;

				if (g_ySI) ySIFillMapsAlt();
			}
		}
		break;
	}
	}
}

extern "C"
{
	BOOL APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved)
	{
		if (fdwReason == DLL_PROCESS_ATTACH)
		{
			yUIHandle = hModule;
			DisableThreadLibraryCalls(hModule);
		}
		return TRUE;
	}

	bool NVSEPlugin_Query(const NVSEInterface* nvse, PluginInfo* info)
	{
		info->infoVersion = PluginInfo::kInfoVersion;
		info->name = "yUI";
		info->version = PLUGIN_VERSION;

//		if (nvse->isNogore)
//		{
//			MessageBoxA(nullptr, "Nogore EXE is not supported!", "yUI", MB_ICONEXCLAMATION);
//			return false;
//		}

		return true;//versionCheck(nvse);
	}

	bool NVSEPlugin_Load(const NVSEInterface* nvse)
	{
		g_nvse = nvse;
		nvse->SetOpcodeBase(0x21D0);

		// setup DirectInput hooks
//		NVSEDataInterface* nvseData = (NVSEDataInterface*)nvse->QueryInterface(kInterface_Data);
//		g_DIHook = (DIHookControl*)nvseData->GetSingleton(NVSEDataInterface::kNVSEData_DIHookControl);

		// setup NVSE message handler
		NVSEMessagingInterface* msgIntfc = (NVSEMessagingInterface*)nvse->QueryInterface(kInterface_Messaging);
		msgIntfc->RegisterListener(nvse->GetPluginHandle(), "NVSE", MessageHandler);

		NVSECommandTableInterface* cmdTable = (NVSECommandTableInterface*)nvse->QueryInterface(kInterface_CommandTable);
		GetCmdByOpcode = cmdTable->GetByOpcode;

//		handleIniOptions();

		REG_CMD_STR(ySIGetTagTrait);
//		REG_CMD_STR(ySITagGetTrait);

		if (nvse->isEditor)
		{
			return true;
		}

		writePatches();

		NVSEStringVarInterface* strInterface = (NVSEStringVarInterface*)nvse->QueryInterface(kInterface_StringVar);
		GetStringVar = strInterface->GetString;
		SetStringVar = strInterface->SetString;
		AssignString = strInterface->Assign;
		NVSEArrayVarInterface* arrInterface = (NVSEArrayVarInterface*)nvse->QueryInterface(kInterface_ArrayVar);
		CreateArray = arrInterface->CreateArray;
		CreateStringMap = arrInterface->CreateStringMap;
		AssignCommandResult = arrInterface->AssignCommandResult;
		SetElement = arrInterface->SetElement;
		AppendElement = arrInterface->AppendElement;
		GetArraySize = arrInterface->GetArraySize;
		LookupArrayByID = arrInterface->LookupArrayByID;
		GetElement = arrInterface->GetElement;
		GetElements = arrInterface->GetElements;
		NVSEScriptInterface* scrInterface = (NVSEScriptInterface*)nvse->QueryInterface(kInterface_Script);
		ExtractArgsEx = scrInterface->ExtractArgsEx;
		ExtractFormatStringArgs = scrInterface->ExtractFormatStringArgs;
		CallFunction = scrInterface->CallFunction;

		return true;
	}
};


bool versionCheck(const NVSEInterface* nvse)
{
	return !(nvse->nvseVersion < NVSE_VERSION_INTEGER);
}

void writePatches()
{
	if (g_ySI) patchRemoveTagsAddIcons();
	if (g_yCM) patchAddyCMToSettingsMenu();
//	patch1080pUI();
}
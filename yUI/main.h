#pragma once
#include <deque>
#include <filesystem>
#include <functional>
#include <set>
#include <unordered_set>
#include <PluginAPI.h>
#include <Utilities.h>
#include "file.h"

#include <ySI.h>
#include <yCM.h>
#include <CommandTable.h>

extern std::deque<std::function<void()>> g_executionQueue;

#define IS_TRANSITION_FIX 0

DebugLog					gLog;

NVSEInterface*				g_nvseInterface = nullptr;
NVSEStringVarInterface*		g_stringInterface = nullptr;
NVSEArrayVarInterface*		g_arrayInterface = nullptr;
NVSEMessagingInterface*		g_messagingInterface = nullptr;
NVSEScriptInterface*		g_scriptInterface = nullptr;
NVSECommandTableInterface*	g_commandInterface = nullptr;
NVSEDataInterface*			g_dataInterface = nullptr;

//DIHookControl* g_DIHook = nullptr;
PlayerCharacter*			g_player = nullptr;
ActorValueOwner*			g_playerAVOwner = nullptr;
BaseProcess*				g_playerProcess = nullptr;
DataHandler*				g_dataHandler = nullptr;

const char*			(*GetStringVar)(UInt32);
void				(*SetStringVar)(UInt32, const char*);
inline bool			(*AssignString)(ParamInfo*, void*, TESObjectREFR*, TESObjectREFR*, Script*, ScriptEventList*, double*, UInt32*, const char*);

inline int iDoOnce = 0;

inline TileMenu* g_HUDMainMenu, * g_StartMenu, * g_BarterMenu, * g_ContainerMenu, * g_RepairMenu, * g_RepairServicesMenu, * g_MapMenu, * g_StatsMenu, * g_InventoryMenu;

inline NiTPointerMap<TESForm>** g_allFormsMap;
inline std::unordered_set<TESForm*> g_CraftingComponents;

namespace SI_Files
{
	inline std::vector<JSONEntryItem>						g_Items_JSON;
	inline std::vector<JSONEntryTag>						g_Tags_JSON;
}

namespace SI
{
	inline std::unordered_map <TESForm*, std::string>		g_Items;
	inline std::unordered_map <std::string, JSONEntryTag>	g_Tags;
	inline std::unordered_set <std::string>					g_Categories;
	inline std::vector<std::filesystem::path>				g_XMLPaths;
}

inline yCM g_yCM_Manager;
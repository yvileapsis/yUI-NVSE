#pragma once
#include <deque>
#include <filesystem>
#include <functional>
#include <set>
#include <unordered_set>
#include <PluginAPI.h>
#include <internal/DecodedUI.h>

#include "file.h"

#include <ySI.h>
#include <yCM.h>
#include <CommandTable.h>

extern std::deque<std::function<void()>> g_executionQueue;

#define IS_TRANSITION_FIX 0

bool IsGodMode();

IDebugLog		gLog("yUI.log");

NVSEInterface*				g_nvseInterface = nullptr;
NVSEStringVarInterface*		g_stringInterface = nullptr;
NVSEArrayVarInterface*		g_arrayInterface = nullptr;
NVSEMessagingInterface*		g_messagingInterface = nullptr;
NVSEScriptInterface*		g_scriptInterface = nullptr;
NVSECommandTableInterface*	g_commandInterface = nullptr;
NVSEDataInterface*			g_dataInterface = nullptr;

//DIHookControl* g_DIHook = nullptr;
PlayerCharacter*	g_player = nullptr;
ActorValueOwner*	g_playerAVOwner = nullptr;
BaseProcess*		g_playerProcess = nullptr;
DataHandler*		g_dataHandler = nullptr;

const char*			(*GetStringVar)(UInt32);
void				(*SetStringVar)(UInt32, const char*);
inline bool			(*AssignString)(ParamInfo*, void*, TESObjectREFR*, TESObjectREFR*, Script*, ScriptEventList*, double*, UInt32*, const char*);

inline int iDoOnce = 0;

inline TileMenu* g_HUDMainMenu, * g_StartMenu, * g_BarterMenu, * g_ContainerMenu, * g_RepairMenu, * g_RepairServicesMenu, * g_MapMenu, * g_StatsMenu, * g_InventoryMenu;

tList<char> queuedConsoleMessages;

inline NiTPointerMap<TESForm>** g_allFormsMap;
inline TileMenu** g_tileMenuArray;

inline std::vector<JSONEntryItem> g_SI_Items_JSON;
inline std::vector<JSONEntryTag> g_SI_Tags_JSON;

inline std::vector<std::filesystem::path> g_XMLPaths;

inline std::unordered_set<TESForm*> g_CraftingComponents;

inline std::unordered_map <TESForm*, std::string> g_SI_Items;
inline std::unordered_map <std::string, JSONEntryTag> g_SI_Tags;
inline std::unordered_set <std::string> g_SI_Categories;

inline int iInjected = 0;

inline yCM g_yCM_Manager;
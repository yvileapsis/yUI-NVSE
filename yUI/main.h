#pragma once
#include <deque>
#include <filesystem>
#include <functional>
#include <set>
#include <unordered_set>
#include <PluginAPI.h>
#include <internal/DecodedUI.h>

extern std::deque<std::function<void()>> g_executionQueue;

#define IS_TRANSITION_FIX 0

bool IsGodMode();

IDebugLog		gLog("yUI.log");

NVSEInterface*				g_nvseInterface = nullptr;
NVSEStringVarInterface*		g_stringInterface = nullptr;
NVSEArrayVarInterface*		g_arrayInterface = nullptr;
NVSEMessagingInterface*		g_messagingInterface = nullptr;
NVSEScriptInterface*		g_scriptInterface = nullptr;
NVSECommandTableInterface*	g_commandTableInterface = nullptr;
NVSEDataInterface*			g_dataInterface = nullptr;

//DIHookControl* g_DIHook = nullptr;
PlayerCharacter*	g_player = nullptr;
ActorValueOwner*	g_playerAVOwner = nullptr;
BaseProcess*		g_playerProcess = nullptr;
DataHandler*		g_dataHandler = nullptr;

const char*			(*GetStringVar)(UInt32);
void				(*SetStringVar)(UInt32, const char*);
bool				(*AssignString)(ParamInfo*, void*, TESObjectREFR*, TESObjectREFR*, Script*, ScriptEventList*, double*, UInt32*, const char*);

int iDoOnce = 0;

Tile* g_HUDMainMenu, * g_StartMenu, * g_BarterMenu, * g_ContainerMenu, * g_RepairMenu, * g_RepairServicesMenu, * g_MapMenu, * g_StatsMenu;

tList<char> queuedConsoleMessages;

NiTPointerMap<TESForm>** g_allFormsMap;

std::unordered_set<TESForm*> map_CraftingComponents;

std::set <Tile*> map_ySI_Tag;
std::set <Tile*> map_ySI_List;
std::unordered_map <TESForm*, std::string> map_ySI_Item;
std::unordered_map <std::string, Tile*> map_ySI_Icon;
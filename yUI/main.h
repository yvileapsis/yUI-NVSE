#pragma once
#include <deque>
#include <Utilities.h>
#include <PluginAPI.h>
#include <GameData.h>

extern std::deque<std::function<void()>> g_executionQueue;

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
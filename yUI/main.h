#pragma once
#include <deque>
#include <Utilities.h>
#include <PluginAPI.h>
#include <GameData.h>

extern std::deque<std::function<void()>> g_executionQueue;

inline DebugLog						gLog;

inline NVSEInterface*				g_nvseInterface = nullptr;
inline NVSEStringVarInterface*		g_stringInterface = nullptr;
inline NVSEArrayVarInterface*		g_arrayInterface = nullptr;
inline NVSEMessagingInterface*		g_messagingInterface = nullptr;
inline NVSEScriptInterface*			g_scriptInterface = nullptr;
inline NVSECommandTableInterface*	g_commandInterface = nullptr;
inline NVSEDataInterface*			g_dataInterface = nullptr;

inline DIHookControl*				g_DIHook = nullptr;
inline PlayerCharacter*				g_player = nullptr;
inline ActorValueOwner*				g_playerAVOwner = nullptr;
inline BaseProcess*					g_playerProcess = nullptr;
inline DataHandler*					g_dataHandler = nullptr;

const char*			(*GetStringVar)(UInt32);
void				(*SetStringVar)(UInt32, const char*);
inline bool			(*AssignString)(ParamInfo*, void*, TESObjectREFR*, TESObjectREFR*, Script*, ScriptEventList*, double*, UInt32*, const char*);

inline int iDoOnce = 0;

inline TileMenu* g_HUDMainMenu, * g_StartMenu, * g_BarterMenu, * g_ContainerMenu, * g_RepairMenu, * g_RepairServicesMenu, * g_MapMenu, * g_StatsMenu, * g_InventoryMenu;
#pragma once
#include <PluginAPI.h>
#include <GameData.h>
#include <GameObjects.h>
#include <definitions.h>
#include <set>
#include <deque>
#include <functional>

inline DebugLog						gLog;

inline PluginHandle					g_pluginHandle			= kPluginHandle_Invalid;

inline NVSEInterface*				g_nvseInterface			= nullptr;
inline NVSEStringVarInterface*		g_stringInterface		= nullptr;
inline NVSEArrayVarInterface*		g_arrayInterface		= nullptr;
inline NVSEMessagingInterface*		g_messagingInterface	= nullptr;
inline NVSEScriptInterface*			g_scriptInterface		= nullptr;
inline NVSECommandTableInterface*	g_commandInterface		= nullptr;
inline NVSEDataInterface*			g_dataInterface			= nullptr;
inline NVSEEventManagerInterface*	g_eventInterface		= nullptr;

inline DIHookControl*				g_DIHook				= nullptr;
inline PlayerCharacter*				g_player				= nullptr;
inline ActorValueOwner*				g_playerAVOwner			= nullptr;
inline BaseProcess*					g_playerProcess			= nullptr;
inline DataHandler*					g_dataHandler			= nullptr;

inline int iDoOnce = 0;

inline std::deque<std::function<void()>> pluginLoad;
inline std::deque<std::function<void()>> deferredInit;
inline std::deque<std::function<void()>> mainLoop;
inline std::deque<std::function<void()>> mainLoopDoOnce;
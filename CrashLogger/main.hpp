#pragma once
#include <PluginAPI.hpp>
#include <definitions.hpp>

#include <deque>

inline PluginHandle					g_pluginHandle			= kPluginHandle_Invalid;

inline SEInterface*					g_seInterface			= nullptr;
inline NVSEStringVarInterface*		g_stringInterface		= nullptr;
inline NVSEArrayVarInterface*		g_arrayInterface		= nullptr;
inline NVSEMessagingInterface*		g_messagingInterface	= nullptr;
inline NVSEScriptInterface*			g_scriptInterface		= nullptr;
inline NVSECommandTableInterface*	g_commandInterface		= nullptr;
inline NVSEDataInterface*			g_dataInterface			= nullptr;
inline NVSEEventManagerInterface*	g_eventInterface		= nullptr;
inline NVSELoggingInterface*		g_loggingInterface		= nullptr;

inline DIHookControl*				g_DIHook				= nullptr;
inline PlayerCharacter*				g_player				= nullptr;
inline ActorValueOwner*				g_playerAVOwner			= nullptr;
inline TESDataHandler*				g_TESDataHandler		= nullptr;
inline HUDMainMenu*					g_HUDMainMenu			= nullptr;

inline bool bMainLoopDoOnce = false;

enum CurrentGame
{
	kDefault			= 0,
	kOblivion			= 'TESO',
	kFallout3			= 'FO3',
	kFalloutNewVegas	= 'FONV',
};

inline CurrentGame					g_currentGame;

inline std::vector<void(*)()>		pluginLoad;
inline std::vector<void(*)()>		deferredInit;
inline std::vector<void(*)()>		mainLoop;
inline std::vector<void(*)()>		mainLoopDoOnce;

inline std::vector<void(*)()>		onRender;
inline std::vector<void(*)(Actor*)>	onHit;

inline std::vector<void(*)(TESObjectREFR*, TESObjectREFR*, TESForm*)> onAddDrop;
inline std::vector<void(*)(TESObjectREFR*, TESObjectREFR*, TESForm*)> onActivate;
inline std::vector<bool(*)(TESObjectREFR*, Actor*, bool)> onPreActivate;
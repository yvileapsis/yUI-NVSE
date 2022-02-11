#pragma once
#include <PluginAPI.h>
#include <GameData.h>

inline DebugLog						gLog;

inline NVSEInterface*				g_nvseInterface			= nullptr;
inline NVSEStringVarInterface*		g_stringInterface		= nullptr;
inline NVSEArrayVarInterface*		g_arrayInterface		= nullptr;
inline NVSEMessagingInterface*		g_messagingInterface	= nullptr;
inline NVSEScriptInterface*			g_scriptInterface		= nullptr;
inline NVSECommandTableInterface*	g_commandInterface		= nullptr;
inline NVSEDataInterface*			g_dataInterface			= nullptr;

inline DIHookControl*				g_DIHook				= nullptr;
inline PlayerCharacter*				g_player				= nullptr;
inline ActorValueOwner*				g_playerAVOwner			= nullptr;
inline BaseProcess*					g_playerProcess			= nullptr;
inline DataHandler*					g_dataHandler			= nullptr;

inline int iDoOnce = 0;
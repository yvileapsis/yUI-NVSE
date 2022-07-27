#pragma once
#include <GameUI.h>
#include <GameObjects.h>
#include <GameProcess.h>

#include <unordered_set>
#include <set>
#include <unordered_map>
#include <map>
#include <ranges>
#include <cmath>
#include <chrono>

inline HUDMainMenu*			g_MenuHUDMain			= nullptr;
inline Tile*				g_TileReticleCenter		= nullptr;
extern PlayerCharacter*		g_player;

void InitFunctions();

__forceinline bool GetPCUsingIronSights()
{
	return g_player->ironSightNode && g_player->baseProcess->IsWeaponOut() || g_player->baseProcess->IsAiming();
}

__forceinline bool GetPCUsingScope()
{
	return g_MenuHUDMain->isUsingScope;
}

typedef bool (*_JG_WorldToScreen)(NiPoint3* posXYZ, NiPoint3& posOut, float offscreenHandling);
inline _JG_WorldToScreen JG_WorldToScreen;

Float32 GetJIPAuxVarOrDefault(const char* auxvar, SInt32 index, Float32 def);
Float32 SetJIPAuxVarOrDefault(const char* auxvar, SInt32 index, Float32 value);

bool IsKeyPressed(UInt32 key, UInt32 flags = 0);
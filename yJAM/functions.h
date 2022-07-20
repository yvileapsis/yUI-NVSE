#pragma once
#include <GameUI.h>
#include <GameObjects.h>
#include <GameProcess.h>

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

Float32 GetObjectDimensions(TESObjectREFR* object);
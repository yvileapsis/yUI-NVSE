#pragma once
#include <GameTiles.h>
#include <GameExtraData.h>
#include <unordered_set>
#include <GameObjects.h>

inline HUDMainMenu*			g_MenuHUDMain			= nullptr;
inline Tile*				g_TileReticleCenter		= nullptr;
inline PlayerCharacter*		g_Player				= nullptr;

void InitFunctions();
#pragma once
#include <filesystem>
#include <set>
#include <functional>
#include "GameTiles.h"
#include <GameData.h>
#include <internal/DecodedUI.h>

#include "file.h"
//#include "functions.h"

//alignas(16) char s_strValBuffer[0x10000];

extern TileMenu* g_HUDMainMenu, * g_StartMenu, * g_BarterMenu, * g_ContainerMenu, * g_RepairMenu, * g_RepairServicesMenu, * g_MapMenu, * g_StatsMenu;

extern tList<char> queuedConsoleMessages;
extern DataHandler* g_dataHandler;
extern NiTPointerMap<TESForm>** g_allFormsMap;

void TileSetStringValueInjectIconHook();
void InventoryMenuSortingHook();
void BarterContainerMenuSortingHook();
void TileSetStringValueHotkeyHook();
void TileSetStringValueHotkeyHook2();
void InjectTemplates();
void FunnyHook();
void FunnyHook2();
void InjectIconTileLastFix();
//void InjectIconTile(Tile* tile, ContChangesEntry* entry);
#pragma once
#include <GameTiles.h>
#include <GameData.h>
#include <internal/DecodedUI.h>

//alignas(16) char s_strValBuffer[0x10000];

extern TileMenu* g_HUDMainMenu, * g_StartMenu, * g_BarterMenu, * g_ContainerMenu, * g_RepairMenu, * g_RepairServicesMenu, * g_MapMenu, * g_StatsMenu;

extern tList<char> queuedConsoleMessages;
extern DataHandler* g_dataHandler;
extern NiTPointerMap<TESForm>** g_allFormsMap;

void IconInjectTileSetStringValueHook();
void SortingInventoryMenuHook();
void SortingBarterContainerMenuHook();
void IconHotkeyHUDTileSetStringValueHook();
void IconHotkeyPipBoyTileSetStringValueHook();
void InjectTemplates();
bool KeyringHideNonKeys(ContChangesEntry*);
void KeyringHideKeysHook();
void KeyringHideKeysShowCategoriesHook();
void KeyringHideNonKeysHook();
void KeyringAddCategoriesHook();
void InjectIconTileLastFix();
void KeyringRefreshPostStewie();
void ContainerEntryListBoxFilterHook();
void KeyringEnableEquipHook();
void KeyringEnableDropHook();
void __fastcall KeyringEnableCancelHook(Tile* tile, void* dummyEDX, enum TileValues tilevalue, signed int a1);
void __fastcall KeyringPipBoyIconHook(Tile* tile, void* dummyEDX, enum TileValues tilevalue, char * string, int propagate);

//void InjectIconTile(Tile* tile, ContChangesEntry* entry);
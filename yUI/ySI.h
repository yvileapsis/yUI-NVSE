#pragma once
#include <filesystem>
#include <set>
#include <functional>
#include "GameTiles.h"
#include <GameData.h>

//#include "functions.h"

//alignas(16) char s_strValBuffer[0x10000];

extern Tile* g_HUDMainMenu, * g_StartMenu, * g_BarterMenu, * g_ContainerMenu, * g_RepairMenu, * g_RepairServicesMenu, * g_MapMenu, * g_StatsMenu;

extern tList<char> queuedConsoleMessages;
extern DataHandler* g_dataHandler;
extern NiTPointerMap<TESForm>** g_allFormsMap;

extern std::set <Tile*> map_ySI_Tag;
extern std::set <Tile*> map_ySI_List;
extern std::unordered_map <TESForm*, std::string> map_ySI_Item;
extern std::unordered_map <std::string, Tile*> map_ySI_Icon;

void Tile__SetStringValueInjectIconHook();
void InventoryMenuSortingHook();
void BarterContainerMenuSortingHook();
void Tile__SetStringValueHotkeyHook();
void Tile__SetStringValueHotkeyHook2();
void ySIFillMapsAlt();
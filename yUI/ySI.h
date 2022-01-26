#pragma once
#include <GameTiles.h>
#include <GameData.h>
#include <internal/DecodedUI.h>

//alignas(16) char s_strValBuffer[0x10000];

extern TileMenu* g_HUDMainMenu, * g_StartMenu, * g_BarterMenu, * g_ContainerMenu, * g_RepairMenu, * g_RepairServicesMenu, * g_MapMenu, * g_StatsMenu, * g_InventoryMenu;

extern tList<char> queuedConsoleMessages;
extern DataHandler* g_dataHandler;
extern NiTPointerMap<TESForm>** g_allFormsMap;

namespace SI
{
	void InjectTemplates();
	void KeyringRefreshPostStewie();
	std::string GetTagForItem(TESForm* form);
	std::string GetTagForItem(ContChangesEntry* entry);
	bool KeyringHideNonKeys(ContChangesEntry*);
	void InjectIconTileLastFix();
}

namespace SI_Files
{
	bool AssignTagToItem(TESForm* form);
}

namespace SI_Hooks
{
	void IconInjectTileSetStringValueHook();
//	void IconInjectTileSetStringValueHook2();
//	void IconInjectTileSetStringValueHook3();
	void SortingInventoryMenuHook();
	void SortingBarterContainerMenuHook();
	void IconHotkeyHUDTileSetStringValueHook();
	void IconHotkeyPipBoyTileSetStringValueHook();

	void KeyringHideKeysHook();
	void KeyringHideKeysShowCategoriesHook();
	void KeyringHideNonKeysHook();
	void KeyringAddCategoriesHook();

	void ContainerEntryListBoxFilterHookPre();
	void ContainerEntryListBoxFilterHookPost();
	void KeyringEnableEquipHook();
	void KeyringEnableDropHook();


	void __fastcall KeyringEnableCancelHook(Tile* tile, void* dummyEDX, enum TileValues tilevalue, signed int a1);
	void __fastcall KeyringPipBoyIconHook(Tile* tile, void* dummyEDX, enum TileValues tilevalue, char* string, int propagate);
}

//void InjectIconTile(Tile* tile, ContChangesEntry* entry);
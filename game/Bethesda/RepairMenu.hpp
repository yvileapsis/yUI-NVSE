#pragma once
#include "Menu.hpp"

// 8C
class RepairMenu : public Menu			// 1035
{
public:
	RepairMenu();
	~RepairMenu();

	TileRect*	pkTile28;		// 28
	TileImage*	pkTile2C;		// 2C
	TileRect*	pkTile30;		// 30
	TileImage*	pkTile34;		// 34
	TileRect*	pkTile38;		// 38
	TileRect*	pkTile3C;		// 3C
	TileText*	pkTile40;		// 40
	TileText*	pkTile44;		// 44
	TileText*	pkTile48;		// 48
	TileRect*	pkTile4C;		// 4C
	TileRect*	pkTile50;		// 50
	TileImage*	pkTile54;		// 54
	TileImage*	pkTile58;		// 58
	MenuItemEntryList		repairItems;	// 5C

	static RepairMenu* GetSingleton() { return *reinterpret_cast<RepairMenu**>(0x11DA75C); };
	static InventoryChanges* GetSelection() { return *reinterpret_cast<InventoryChanges**>(0x11DA760); };
	static void SetSelection(InventoryChanges* entry) { *reinterpret_cast<InventoryChanges**>(0x11DA760) = entry; };
};

#pragma once
#include "Menu.hpp"

// A0
class RepairServicesMenu : public Menu	// 1058
{
public:
	RepairServicesMenu();
	~RepairServicesMenu();

	TileText*	pkTile28;		// 28
	TileText*	pkTile2C;		// 2C
	TileImage*	pkTile30;		// 30
	TileImage*	pkTile34;		// 34
	TileRect*	pkTile38;		// 38
	TileRect*	pkTile3C;		// 3C
	TileRect*	pkTile40;		// 40
	TileText*	pkTile44;		// 44
	TileText*	pkTile48;		// 48
	TileText*	pkTile4C;		// 4C
	TileRect*	pkTile50;		// 50
	TileRect*	pkTile54;		// 54
	TileText*	pkTile58;		// 58
	TileImage*	pkTile5C;		// 5C
	TileImage*	pkTile60;		// 60
	TileImage*	pkTile64;		// 64
	MenuItemEntryList	itemList;		// 68
	UInt32				repairAllCost;	// 98
	UInt8				skill;			// 9C
	UInt8				pad9D[3];		// 9D

	static RepairServicesMenu*	GetSingleton() { return *reinterpret_cast<RepairServicesMenu**>(0x11DA7F0); };
	static Actor*				GetActor() { return *reinterpret_cast<Actor**>(0x11DA7F4); };
	void __forceinline			Refresh() { ThisCall(0x7B8B30, this); }
};

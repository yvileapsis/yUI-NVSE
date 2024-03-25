#pragma once
#include "Menu.hpp"
#include "ListBox.hpp"
#include "HotKeyWheel.hpp"

class ItemChange;

class InventoryMenu : public Menu		// 1002
{
public:
	InventoryMenu();
	~InventoryMenu() override;

	enum Filter : UInt32
	{
		kWeapons,
		kApparel,
		kAid,
		kMisc,
		kAmmo
	};

	enum InventoryMenuHotkey
	{
		Inv_HotKeyWheel			= 0x5,
		Inv_Equip				= 0x6,
		Inv_Drop				= 0x7,
		Inv_Repair				= 0x8,
		Inv_HotkeyButton		= 0x9,
		Inv_Cancel				= 0xA,
		Inv_ModMenu				= 0x13,
	};

	enum Buttons
	{
		kButton_HotKeyWheel		= 0x5,
		kButton_Equip			= 0x6,
		kButton_Drop			= 0x7,
		kButton_Repair			= 0x8,
		kButton_HotkeyButton	= 0x9,
		kButton_Cancel			= 0xA,
		kButton_ModMenu			= 0x13,
	};

	union
	{
		Tile*					pkTiles[23];
		struct
		{
			TileRect*			pkTileCapsInfo;
			TileRect*			pkTilePlayerHPInfo;
			TileRect*			pkTilePlayerDRInfo;
			TileRect*			pkTilePlayerWGInfo;
			TileImage*			pkTileInventoryList;
			TileRect*			pkTileHotKeyWheel;
			TileImage*			pkTileEquipButton;
			TileImage*			pkTileDropButton;
			TileImage*			pkTileRepairButton;
			TileImage*			pkTileHotkeyButton;
			TileImage*			pkTileCancelButton;
			TileImage*			pkTileItemIcon;
			TileRect*			pkTileItemInfoRect;
			TileRect*			pkTileTabline;
			TileRect*			pkTileDAMInfo;
			TileRect*			pkTileDPSInfo;
			TileRect*			pkTileStrengthReqInfo;
			TileRect*			pkTileDamResistInfo;
			TileRect*			pkTileDamThresholdInfo;
			TileImage*			pkTileModButton;
			TileImage*			pkTileItemIconBadge;
			TileRect*			pkTilePlayerDTInfo;
			TileText*			pkTileStrReq;
		};
	};

	struct ScrollPos
	{
		SInt32					listIndex;
		SInt32					currentValue;
	};

	Filter						eFilter;				// 084
	ScrollPos					kTabScrollPositions[6];	// 088, the scroll index for Weapons, Apparel, Aid, Misc, Ammo and the Keyring
	ListBox<ItemChange>			kItemsList;				// 0B8
	HotKeyWheel					kHotkeyWheel;			// 0E8
	BSSimpleList<ItemChange*>	kChangedItemsList;		// 11C

	static InventoryMenu*		GetSingleton() { return *reinterpret_cast<InventoryMenu**>(0x11D9EA4); }
	static UInt32				GetMenuID() { return 1002; }
	static ItemChange*			GetSelection() { return *reinterpret_cast<ItemChange**>(0x11D9EA8); }
	bool						IsKeyringOpen() const;
	static void					RestoreScrollPosition() { CdeclCall(0x7800C0); }
	static bool					ShouldHideItem(ItemChange* entry) { return CdeclCall<bool>(0x782620, entry); }
	static SInt32				CompareForSorting(const ListBoxItem<ItemChange>* a1, const ListBoxItem<ItemChange>* a2) { return CdeclCall<SInt32>(0x7824E0, a1, a2); }
	void						ResetInventorySelectionAndHideDataTile() { ThisCall(0x781B10, this); }
};
static_assert(sizeof(InventoryMenu) == 0x124);
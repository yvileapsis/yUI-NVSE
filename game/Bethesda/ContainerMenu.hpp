#pragma once
#include "Menu.hpp"
#include "ListBox.hpp"
#include "BSSoundHandle.hpp"

class ItemChange;

// 10C
class ContainerMenu : public Menu		// 1008
{
public:
	ContainerMenu();
	~ContainerMenu();

	enum Mode : UInt32
	{
		kNormal = 0x1,
		kPickpocket,
		kTeammate,
		kRockItLauncher
	};

	enum Buttons
	{
		CM_Items_LeftFilterArrow = 0x0,
		CM_ItemsTitle = 0x1,
		CM_Items_RightFilterArrow = 0x2,
		CM_Items_CapsLabel = 0x3,
		CM_Items_InventoryList = 0x4,
		CM_Container_LeftFilterArrow = 0x5,
		CM_ContainerTitle = 0x6,
		CM_Container_RightFilterArrow = 0x7,
		CM_Container_InventoryList = 0x8,
		CM_ArrowIcon = 0x9,
		CM_TakeAllButton = 0xA,
		CM_ExitButton = 0xB,
		CM_ItemIcon = 0xC,
		CM_ItemData = 0xD,
		CM_list_template_container = 0x14,
	};

	enum Filter : SInt32
	{
		kAllItems,
		kWeapons,
		kArmor,
		kAid,
		kMisc,
		kAmmo,
		kMAX = kAmmo,
	};

	union
	{
		Tile*					pkTiles[19];
		struct
		{
			TileImage*			pkTile028;			// 028
			TileText*			itemsTitle;			// 02C
			TileImage*			pkTile030;			// 030
			TileText*			pkTileInventoryWeight;	// 034
			TileImage*			pkTile038;			// 038
			TileImage*			pkTile03C;			// 03C
			TileText*			containerTitle;		// 040
			TileImage*			pkTile044;			// 044
			TileImage*			pkTile048;			// 048
			TileImage*			pkTile04C;			// 04C
			TileImage*			takeAllTile;		// 050
			TileImage*			pkTile054;			// 054
			TileImage*			pkTile058;			// 058
			TileRect*			pkTile05C;			// 05C
			TileRect*			pkTile060;			// 060
			TileRect*			pkTile064;			// 064
			TileRect*			pkTile068;			// 068
			TileRect*			pkTile06C;			// 06C
			TileRect*			pkTile070;			// 070
		};
	};
	TESObjectREFR*				pkContainer;		// 074
	BSSimpleList<void*>			list078;			// 078
	Mode						eMode;				// 080
	UInt32						uiValueTransfered;	// 084
	UInt8						bHasPickedPocket;	// 088
	UInt8						bHasFailedPickpocket;	// 089
	UInt8						pad08A;				// 08A
	UInt8						pad08B;				// 08B
	Filter						eLeftFilter;		// 08C
	Filter						eRightFilter;		// 090
	UInt32						uiMenuSoundID;		// 094
	ListBox<ItemChange*>		kLeftItems;			// 098
	ListBox<ItemChange*>		kRightItems;		// 0C8
	ListBox<ItemChange*>*		pkCurrentItems;		// 0F8
	UInt32						unk0FC;				// 0FC
	BSSoundHandle				kMenuSound;			// 100

	static ContainerMenu*		GetSingleton() { return *reinterpret_cast<ContainerMenu**>(0x11D93F8); }
	static ItemChange*			GetSelection() { return *reinterpret_cast<ItemChange**>(0x11D93FC); }
	static void					SetSelection(ItemChange* entry) { *reinterpret_cast<ItemChange**>(0x11D93FC) = entry; }
	__forceinline void 			Refresh(TESForm* form = nullptr) { ThisCall(0x75C280, form); }

};
static_assert(sizeof(ContainerMenu) == 0x10C);
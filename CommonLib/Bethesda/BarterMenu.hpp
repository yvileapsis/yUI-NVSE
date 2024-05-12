#pragma once
#include "Menu.hpp"
#include "InventoryChanges.hpp"
#include "ListBox.hpp"

// 0x120
class BarterMenu : public Menu			// 1053
{
public:
	BarterMenu();
	~BarterMenu() override;

	enum Buttons
	{
		BM_LeftFilterArrow				= 0x2,
		BM_ItemsTitle					= 0x3,
		BM_RightFilterArrow				= 0x4,
		BM_Container_LeftFilterArrow	= 0x7,
		BM_Container_Title				= 0x8,
		BM_Container_RightFilterArrow	= 0x9,
		BM_ButtonX						= 0xD,
		BM_ButtonB						= 0x13,
		BM_ListBox						= 0x17,
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
		Tile*	pkTile[22];
		struct
		{
			TileImage*					tile028;		// 028
			TileImage*					tile02C;		// 02C
			TileImage*					tile030;		// 030
			TileText*					pkItemsTitle;	// 034
			TileImage*					tile038;		// 038
			TileText*					tile03C;		// 03C
			TileImage*					tile040;		// 040
			TileImage*					tile044;		// 044
			TileText*					pkContainerTitle; // 048
			TileImage*					tile04C;		// 04C
			TileText*					tile050;		// 050
			TileImage*					tile054;		// 054
			TileRect*					tile058;		// 058
			TileImage*					tile05C;		// 05C
			TileRect*					tile060;		// 060
			TileRect*					tile064;		// 064
			TileRect*					tile068;		// 068
			TileRect*					tile06C;		// 06C
			TileRect*					tile070;		// 070
			TileImage*					tile074;		// 074
			TileImage*					tile078;		// 078
			TileRect*					tile07C;		// 07C
		};
	};
	TESObjectREFR*						pkMerchantRef;	// 080
	Float32								fBarterTotalSum;// 084
	UInt32								unk088;			// 088
	UInt32								uiPlayerGold;	// 08C
	UInt32								uiMerchantGold;	// 090
	Float32								fBuyValueMult;	// 094
	Float32								fSellValueMult;	// 098
	Filter								eLeftFilter;	// 09C
	Filter								eRightFilter;	// 0A0
	UInt32								uiMenuSoundID;	// 0A4
	ListBox<ItemChange*>				kLeftItems;		// 0A8
	ListBox<ItemChange*>				kRightItems;	// 0D8
	ListBox<ItemChange*>*				kCurrentItems;	// 108
	BSSimpleList<ItemChange*>			kLeftBarter;	// 10C
	BSSimpleList<ItemChange*>			kRightBarter;	// 114
	UInt32								unk11C;			// 11C

	static BarterMenu* GetSingleton() { return *reinterpret_cast<BarterMenu**>(0x11D8FA4); }
	void __forceinline Refresh(TESForm* form = nullptr) { ThisCall(0x72DC30, form); }
};
static_assert(sizeof(BarterMenu) == 0x120);
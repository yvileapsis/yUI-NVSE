#pragma once
#include "Menu.hpp"
#include "ListBox.hpp"

// 0x90
class ItemModMenu : Menu
{
	~ItemModMenu();
	ItemModMenu();

public:
	union
	{
		Tile* pkTiles[14];
		struct
		{
			TileRect*	pkTileItemName;	
			TileImage*	pkTileItemModList;
			TileRect*	pkTileLeftBracket;
			TileImage*	pkTileItemIcon;
			TileRect*	pkTileItemHealth;
			TileRect*	pkTileItemStat;
			TileText*	pkTileHealthImprovementText;
			TileText*	pkTileStatImprovementText;
			TileText*	pkTileChooseItemText;
			TileRect*	pkTileModItemHealth;
			TileRect*	pkTileModItemStat;
			TileImage*	pkTileItemModButton;
			TileImage*	pkTileCancelButton;
			TileText*	pkTileModDesc;
		};
	};
	MenuItemEntryList kItemModList;

	static ItemModMenu* GetSingleton() { return *reinterpret_cast<ItemModMenu**>(0x11D9F54); };
	static InventoryChanges* GetTarget() { return *reinterpret_cast<InventoryChanges**>(0x11D9F58); };
};
static_assert(sizeof(ItemModMenu) == 0x90);
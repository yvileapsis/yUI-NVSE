#pragma once
#include "Menu.hpp"
#include "ListBox.hpp"

// 0x8C
class TraitMenu : public Menu
{
public:
	TraitMenu();
	~TraitMenu();

	union
	{
		Tile* pkTiles[9];
		struct
		{
			TileText*	pkTileTitleText;
			TileImage*	pkTile2C;
			TileImage*	pkTile30;
			TileText*	pkTile34;
			TileText*	pkTile38;
			TileImage*	pkTileReset;
			TileImage*	pkTileTitle;
			TileImage*	pkTile44;
			TileImage*	pkTile48;
		};
	};

	UInt32					uiNumSelected;
	UInt32					uiMaxSelect;
	ListBox<BGSPerk>		kPerkListBox;
	BSSimpleList<BGSPerk*>	kPerkList;

	static TraitMenu* GetSingleton() { return *reinterpret_cast<TraitMenu**>(0x11DAF74); };
};
static_assert(sizeof(TraitMenu) == 0x8C);
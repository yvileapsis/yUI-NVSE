#pragma once
#include "Menu.hpp"

class BookMenu : public Menu
{
public:
	BookMenu();
	~BookMenu();

	TileImage*		tile28;
	TileImage*		tile2C;
	Actor*			activator;
	TESObjectBOOK*	bookObject;
	UInt32			unk38;
};

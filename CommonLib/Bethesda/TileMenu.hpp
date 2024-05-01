#pragma once
#include "TileRect.hpp"

// 40
class TileMenu : public TileRect
{
public:
	Menu*	pkParentMenu;	// 3C - confirmed
};
static_assert(sizeof(TileMenu) == 0x40);
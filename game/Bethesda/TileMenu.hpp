#pragma once

#include "TileRect.hpp"

class Menu;

class TileMenu : public TileRect {
public:
	Menu* menu;
};

ASSERT_SIZE(TileMenu, 0x40);
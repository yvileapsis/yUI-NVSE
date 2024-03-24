#pragma once

#include "Tile.hpp"

class TileRect : public Tile {
public:
	UInt32	unk38;
};

ASSERT_SIZE(TileRect, 0x3C);
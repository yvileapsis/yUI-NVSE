#pragma once
#include "Tile.hpp"

// 3C
class TileRect : public Tile
{
public:
	UInt32	unk38;	// 38
};
static_assert(sizeof(TileRect) == 0x3C);
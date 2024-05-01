#pragma once
#include "BSString.hpp"
#include "Tile.hpp";

// 0x50
class Tile3D : Tile
{
	UInt32		unk38;
	UInt32		unk3C;
	BSStringT	str40;
	BSStringT	maybeModelPath;
};
static_assert(sizeof(Tile3D) == 0x50);
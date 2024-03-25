#pragma once
#include "TileImage.hpp"

// 48
class RadialTile : public TileImage
{
	enum
	{
		xOffset = kTileValue_user0,
		yOffset = kTileValue_user1,
		anglemin = kTileValue_user2,
		anglemax = kTileValue_user3,
		distanceMin = kTileValue_user4,
		distanceMax = kTileValue_user5
	};
};
static_assert(sizeof(RadialTile) == 0x48);
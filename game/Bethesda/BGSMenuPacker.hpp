#pragma once

#include "NiTArray.hpp"
#include "NiPoint2.hpp"

class Tile;

class BGSMenuPacker {
public:
	struct PackedMenu {
		NiPoint2	kPosition;
		NiPoint2	kDimensions;
		NiPoint2	kOrgPosition;
		Tile*		pTile;
	};

	NiTPrimitiveArray<PackedMenu*>	kMenus;
	BYTE							gap10[17];
	BYTE							byte21;
};

ASSERT_SIZE(BGSMenuPacker, 0x24);
#pragma once
#include "NiExtraData.hpp"

class Tile;
class NiNode;

class TileExtra : public NiExtraData
{
public:
	TileExtra();
	~TileExtra();

	Tile* parentTile;	// 0C
	NiNode* parentNode;	// 10
};
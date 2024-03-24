#pragma once

#include "Tile.hpp"

class NiTexture;
class TileShaderProperty;

class TileImage : public Tile {
public:
	float							flt038;
	NiPointer<NiTexture>			spTexture;
	NiPointer<TileShaderProperty>	spShaderProp;
	UInt8							byt044;
};

ASSERT_SIZE(TileImage, 0x48);
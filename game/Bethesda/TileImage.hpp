#pragma once
#include "Tile.hpp"

class TileShaderProperty;

// 0x48
class TileImage : public Tile
{
public:
	Float32				flt038;		// 38
	RefNiObject			unk03C;		// 3C nirefobject
	TileShaderProperty* shaderProp;		// 40 nireftobject
	UInt8				byt044;		// 44
	UInt8				fill[3];	// 45-47
};
static_assert(sizeof(TileImage) == 0x48);
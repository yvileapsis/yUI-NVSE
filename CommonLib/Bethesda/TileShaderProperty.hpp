#pragma once

#include "BSShaderProperty.hpp"
#include "BSString.hpp"

NiSmartPointer(TileShaderProperty);

class TileShaderProperty : public BSShaderProperty {
public:
	TileShaderProperty();
	virtual ~TileShaderProperty();

	NiTexturePtr						spSrcTexture;
	NiTexturePtr						spAlphaTexture;
	NiColorA							kOverlayColor;
	float								fTileAlpha;
	NiPoint4							kTexScroll;
	NiTexturingProperty::ClampMode	eClampMode;
	bool								byte90;
	bool								bRotates;
	bool								bHasVtxColors;
	bool								bNoTexture;
	BSStringT							strTexturePath;
	RECT								kScissorRect;
	bool								bUseScissorTest;

	CREATE_OBJECT(TileShaderProperty, 0xBB7EF0);
};

ASSERT_SIZE(TileShaderProperty, 0xB0);
#pragma once

#include "BSShaderLightingProperty.hpp"

class GeometryDecalShaderProperty : public BSShaderLightingProperty {
public:
	NiTexturePtr	spTexture;
	DWORD			dword80[21];
};

ASSERT_SIZE(GeometryDecalShaderProperty, 0xD4)
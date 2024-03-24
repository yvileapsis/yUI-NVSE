#pragma once

#include "ImageSpaceEffect.hpp"

class ImageSpaceEffectVolumetricFog : public ImageSpaceEffect {
public:
	NiColorA kFogColor;
};

ASSERT_SIZE(ImageSpaceEffectVolumetricFog, 0x68);
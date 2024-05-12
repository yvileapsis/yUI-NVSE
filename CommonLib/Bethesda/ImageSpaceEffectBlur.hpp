#pragma once

#include "ImageSpaceEffect.hpp"

class ImageSpaceEffectBlur : public ImageSpaceEffect {
public:
	int iRadius;
	bool bBrightPass;

	static void SetBlurRadius(float afRadius) { *(float*)0x12003C4 = afRadius; }
	static float GetBlurRadius() { return *(float*)0x12003C4; }
};

ASSERT_SIZE(ImageSpaceEffectBlur, 0x60)
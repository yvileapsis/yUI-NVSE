#pragma once

#include "ImageSpaceModifierInstance.hpp"

class ImageSpaceModifierInstanceTemp : public ImageSpaceModifierInstance {
public:
	ImageSpaceModifierInstanceTemp();
	virtual ~ImageSpaceModifierInstanceTemp();

	float fDuration;

	NiNewRTTI(ImageSpaceModifierInstanceTemp, ImageSpaceModifierInstance);
};

ASSERT_SIZE(ImageSpaceModifierInstanceTemp, 0x20);
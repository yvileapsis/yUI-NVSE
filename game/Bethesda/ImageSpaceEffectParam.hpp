#pragma once

#include "NiTArray.hpp"
#include "NiTexturingProperty.hpp"

class ImageSpaceEffectParam {
public:
	ImageSpaceEffectParam();
	virtual ~ImageSpaceEffectParam();

	NiTPrimitiveArray<UInt32>							eTextureType;
	NiTPrimitiveArray<NiTexturingProperty::FilterMode>	eFilterMode;

	static ImageSpaceEffectParam* Create(ImageSpaceEffectParam* apThis);
};

ASSERT_SIZE(ImageSpaceEffectParam, 0x24);
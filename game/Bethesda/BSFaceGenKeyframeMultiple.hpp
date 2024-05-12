#pragma once

#include "BSFaceGenKeyframe.hpp"

class BSFaceGenKeyframeMultiple : public BSFaceGenKeyframe {
public:
	BSFaceGenKeyframeMultiple();
	virtual ~BSFaceGenKeyframeMultiple();

	float* pfMorphValues;
	UInt32 uiNumMorphs;
};

ASSERT_SIZE(BSFaceGenKeyframeMultiple, 0x14)
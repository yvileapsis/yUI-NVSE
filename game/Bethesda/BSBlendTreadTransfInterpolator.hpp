#pragma once

#include "NiBlendInterpolator.hpp"

class BSBlendTreadTransfInterpolator : public NiBlendInterpolator {
public:
	BSBlendTreadTransfInterpolator();
	virtual ~BSBlendTreadTransfInterpolator();

	float fCurrLerp;
};

ASSERT_SIZE(BSBlendTreadTransfInterpolator, 0x34);
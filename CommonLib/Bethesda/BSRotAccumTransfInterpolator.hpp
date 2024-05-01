#pragma once

#include "NiTransformInterpolator.hpp"
#include "NiPoint4.hpp"

class BSRotAccumTransfInterpolator : public NiTransformInterpolator {
public:
	NiQuaternion kPreviousRotation;

	NIRTTI_ADDRESS(0x11F3EE0);
};

ASSERT_SIZE(BSRotAccumTransfInterpolator, 0x58);
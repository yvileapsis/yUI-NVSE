#pragma once

#include "hkUnaryActionCinfo.hpp"

class bhkLiquidActionCInfo : public hkUnaryActionCinfo {
public:
	float fInitialStickForce;
	float fStickStrength;
	float fNeighborDistance;
	float fNeighborStrength;
};

ASSERT_SIZE(bhkLiquidActionCInfo, 0x18);
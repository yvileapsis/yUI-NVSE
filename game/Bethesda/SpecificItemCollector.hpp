#pragma once

#include "hkpClosestRayHitCollector.hpp"

class SpecificItemCollector : public hkpClosestRayHitCollector {
public:
	DWORD dword70;
	float unk74;
};

ASSERT_SIZE(SpecificItemCollector, 0x80);
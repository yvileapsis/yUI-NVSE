#pragma once

#include "hkpRayHitCollector.hpp"
#include "hkpWorldRayCastOutput.hpp"

class hkpClosestRayHitCollector : public hkpRayHitCollector {
public:
	hkpWorldRayCastOutput m_rayHit;
};

ASSERT_SIZE(hkpClosestRayHitCollector, 0x70);
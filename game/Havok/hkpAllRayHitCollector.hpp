#pragma once

#include "hkpRayHitCollector.hpp"
#include "hkInplaceArray.hpp"
#include "hkpWorldRayCastOutput.hpp"

ALIGN16 class hkpAllRayHitCollector : public hkpRayHitCollector {
public:
	hkInplaceArray<hkpWorldRayCastOutput, 8> m_hits;

	static hkpAllRayHitCollector* Create(hkpAllRayHitCollector* apThis);
};

ASSERT_SIZE(hkpAllRayHitCollector, 0x320)
#pragma once

#include "hkpShapeRayCastCollectorOutput.hpp"

class hkpCdBody;

class hkpRayHitCollector {
public:
	hkpRayHitCollector();
	virtual void addRayHit(const hkpCdBody& cdBody, const hkpShapeRayCastCollectorOutput& hitInfo) = 0;
	virtual ~hkpRayHitCollector();

	float m_earlyOutHitFraction;
};

ASSERT_SIZE(hkpRayHitCollector, 0x8);
#pragma once

#include "hkpShapeRayCastCollectorOutput.hpp"

ALIGN16 class hkpShapeRayCastOutput : public hkpShapeRayCastCollectorOutput {
public:
	UInt32 m_shapeKeys[8];
	SInt32 m_shapeKeyIndex;
};

ASSERT_SIZE(hkpShapeRayCastOutput, 0x50);
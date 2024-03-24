#pragma once

#include "hkVector4.hpp"

ALIGN16 class hkpShapeRayCastCollectorOutput {
public:
	hkVector4	m_normal;
	float		m_hitFraction;
	int			m_extraInfo;
};

ASSERT_SIZE(hkpShapeRayCastCollectorOutput, 0x20);
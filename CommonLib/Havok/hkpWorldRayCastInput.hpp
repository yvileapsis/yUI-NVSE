#pragma once

#include "hkVector4.hpp"

class hkpWorldRayCastInput {
public:
	hkVector4	m_from;
	hkVector4	m_to;
	bool		m_enableShapeCollectionFilter;
	UInt32		m_filterInfo;
	UInt32		m_userData;
};

ASSERT_SIZE(hkpWorldRayCastInput, 0x30);
#pragma once

#include "hkpCdBody.hpp"
#include "hkContactPoint.hpp"

class hkCdBody;

ALIGN16 struct hkpCdPoint {
	hkContactPoint	m_contact;
	hkpCdBody&		m_cdBodyA;
	hkpCdBody&		m_cdBodyB;
};

ASSERT_SIZE(hkpCdPoint, 0x30);
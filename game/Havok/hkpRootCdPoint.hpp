#pragma once

#include "hkContactPoint.hpp"

class hkpCollidable;

ALIGN16 class hkpRootCdPoint {
	hkContactPoint			m_contact;
	const hkpCollidable*	m_rootCollidableA;
	UInt32					m_shapeKeyA;
	const hkpCollidable*	m_rootCollidableB;
	UInt32					m_shapeKeyB;
};

ASSERT_SIZE(hkpRootCdPoint, 0x30);
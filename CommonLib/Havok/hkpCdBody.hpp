#pragma once

class hkpShape;

class hkpCdBody {
public:
	hkpShape*	m_shape;
	UInt32		m_shapeKey;
	void*		m_motion;
	hkpCdBody*	m_parent;
};

ASSERT_SIZE(hkpCdBody, 0x10);
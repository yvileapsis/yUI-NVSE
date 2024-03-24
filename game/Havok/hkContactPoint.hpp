#pragma once

#include "hkVector4.hpp"

class hkContactPoint {
public:
	hkVector4 m_position;
	hkVector4 m_separatingNormal;
};

ASSERT_SIZE(hkContactPoint, 0x20);
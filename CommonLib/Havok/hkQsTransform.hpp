#pragma once

#include "hkVector4.hpp"
#include "hkQuaternion.hpp"

class hkQsTransform {
public:
	hkVector4		m_translation;
	hkQuaternion	m_rotation;
	hkVector4		m_scale;
};

ASSERT_SIZE(hkQsTransform, 0x30);
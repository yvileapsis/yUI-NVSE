#pragma once

#include "hkQuaternion.hpp"
#include "hkVector4.hpp"

class hkSweptTransform {
public:
	hkVector4		m_centerOfMass0;
	hkVector4		m_centerOfMassWorld;
	hkQuaternion	m_rotation0;
	hkQuaternion	m_rotation1;
	hkVector4		m_centerOfMassLocal;
};

ASSERT_SIZE(hkSweptTransform, 0x50);
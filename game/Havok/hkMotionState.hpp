#pragma once

#include "hkTransform.hpp"
#include "hkSweptTransform.hpp"


class hkMotionState {
public:
	hkTransform			m_transform;
	hkSweptTransform	m_sweptTransform;
	hkVector4			m_deltaAngle;
	float				m_objectRadius;
	float				m_linearDamping;
	float				m_angularDamping;
	UInt8				m_maxLinearVelocity;
	UInt8				m_maxAngularVelocity;
	UInt8				m_deactivationClass;
	UInt8				byteAF;
};

ASSERT_SIZE(hkMotionState, 0xB0);
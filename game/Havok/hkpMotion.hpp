#pragma once

#include "hkReferencedObject.hpp"
#include "hkMotionState.hpp"

class hkpMotion : public hkReferencedObject {
public:
	UInt8			m_type;
	UInt8			m_deactivationIntegrateCounter;
	UInt16			m_deactivationNumInactiveFrames[2];
	hkMotionState	m_motionState;
	hkVector4		m_inertiaAndMassInv;
	hkVector4		m_linearVelocity;
	hkVector4		m_angularVelocity;
	hkVector4		m_deactivationRefPosition[2];
	UInt32			m_deactivationRefOrientation[2];
	UInt32*			m_savedMotion;
	UInt32			m_gravityFactor;
};

ASSERT_SIZE(hkpMotion, 0x120);
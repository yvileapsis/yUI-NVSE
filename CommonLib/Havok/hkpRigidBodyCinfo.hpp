#pragma once

#include "hkVector4.hpp"
#include "hkQuaternion.hpp"
#include "hkMatrix3.hpp"

class hkpShape;

struct hkpRigidBodyCinfo {
	UInt32			m_collisionFilterInfo;
	hkpShape*		m_shape;
	void*			m_localFrame;
	SInt8			m_collisionResponse;
	UInt16			m_contactPointCallbackDelay;
	hkVector4		m_position;
	hkQuaternion	m_rotation;
	hkVector4		m_linearVelocity;
	hkVector4		m_angularVelocity;
	hkMatrix3		m_inertiaTensor;
	hkVector4		m_centerOfMass;
	float			m_mass;
	float			m_linearDamping;
	float			m_angularDamping;
	float			m_gravityFactor;
	float			m_friction;
	float			m_restitution;
	float			m_maxLinearVelocity;
	float			m_maxAngularVelocity;
	float			m_allowedPenetrationDepth;
	SInt8			m_motionType;
	bool			m_enableDeactivation;
	SInt8			m_solverDeactivation;
	SInt8			m_qualityType;
	SInt8			m_autoRemoveLevel;
	UInt8			m_responseModifierFlags;
	SInt8			m_numShapeKeysInContactPointProperties;
	bool			m_forceCollideOntoPpu;
};

ASSERT_SIZE(hkpRigidBodyCinfo, 0xC0);
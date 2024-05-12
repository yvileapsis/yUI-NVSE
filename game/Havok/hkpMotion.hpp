#pragma once

#include "hkReferencedObject.hpp"
#include "hkMotionState.hpp"
#include "hkMatrix3.hpp"

class hkpMotion : public hkReferencedObject {
public:
	hkpMotion();
	virtual ~hkpMotion();

	virtual void setMass(float m);
	virtual void setMassInv(float mInv);
	virtual void getInertiaLocal(hkMatrix3& inertiaOut) const;
	virtual void getInertiaWorld(hkMatrix3& inertiaOut) const;
	virtual void setInertiaLocal(const hkMatrix3& inertia);
	virtual void setInertiaInvLocal(const hkMatrix3& inertiaInv);
	virtual void getInertiaInvLocal(hkMatrix3& inertiaInvOut) const;
	virtual void getInertiaInvWorld(hkMatrix3& inertiaInvOut) const;
	virtual void setCenterOfMassInLocal(const hkVector4& centerOfMass);
	virtual void setPosition(const hkVector4& position);
	virtual void setRotation(const hkQuaternion& rotation);
	virtual void setPositionAndRotation(const hkVector4& position, const hkQuaternion& rotation);
	virtual void setTransform(const hkTransform& transform);
	virtual void setLinearVelocity(const hkVector4& newVel);
	virtual void setAngularVelocity(const hkVector4& newVel);
	virtual void getProjectedPointVelocity(const hkVector4& p, const hkVector4& normal, float& velOut, float& invVirtMassOut) const;
	virtual void applyLinearImpulse(const hkVector4& imp);
	virtual void applyPointImpulse(const hkVector4& imp, const hkVector4& p);
	virtual void applyAngularImpulse(const hkVector4& imp);
	virtual void applyForce(const float deltaTime, const hkVector4& force);
	virtual void applyForceAlt(const float deltaTime, const hkVector4& force, const hkVector4& p);
	virtual void applyTorque(const float deltaTime, const hkVector4& torque);
	virtual void getMotionStateAndVelocitiesAndDeactivationType(hkpMotion* motionOut);

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
#pragma once

#include "hkpRootCdPoint.hpp"
#include "hkArray.hpp"

class hkpCharacterProxy;
class hkpRootCdPoint;
class hkpSimplexSolverInput;
class hkpRigidBody;

struct hkpCharacterObjectInteractionEvent {
	hkVector4		m_position;
	hkVector4		m_normal;
	float			m_objectImpulse;
	float			m_timestep;
	float			m_projectedVelocity;
	float			m_objectMassInv;
	hkpRigidBody*	m_body;
};

struct hkpCharacterObjectInteractionResult {
	hkVector4	m_objectImpulse;
	hkVector4	m_impulsePosition;
};

class hkpCharacterProxyListener {
public:
	hkpCharacterProxyListener();
	virtual ~hkpCharacterProxyListener();
	virtual void processConstraintsCallback(const hkpCharacterProxy* proxy, const hkArray<hkpRootCdPoint>& manifold, hkpSimplexSolverInput& input);
	virtual void contactPointAddedCallback(const hkpCharacterProxy* proxy, const hkpRootCdPoint& point);
	virtual void contactPointRemovedCallback(const hkpCharacterProxy* proxy, const hkpRootCdPoint& point);
	virtual void characterInteractionCallback(hkpCharacterProxy* proxy, hkpCharacterProxy* otherProxy, const hkContactPoint& contact);
	virtual void objectInteractionCallback(hkpCharacterProxy* proxy, const hkpCharacterObjectInteractionEvent& input, hkpCharacterObjectInteractionResult& output);
};

ASSERT_SIZE(hkpCharacterProxyListener, 0x4);
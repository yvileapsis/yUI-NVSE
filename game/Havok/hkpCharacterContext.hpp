#pragma once

#include "hkReferencedObject.hpp"

class hkpCharacterStateManager;

class hkpCharacterContext : public hkReferencedObject {
public:
	UInt32						m_characterType;
	hkpCharacterStateManager*	m_stateManager;
	UInt32						m_currentState;
	UInt32						m_previousState;
	bool						m_filterEnable;
	float						m_maxLinearAcceleration;
	float						m_maxLinearVelocity;
	float						m_gain;
};

ASSERT_SIZE(hkpCharacterContext, 0x28);
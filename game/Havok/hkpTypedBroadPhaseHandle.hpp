#pragma once

#include "hkpBroadPhaseHandle.hpp"

class hkpTypedBroadPhaseHandle : public hkpBroadPhaseHandle {
public:
	SInt8	m_type;
	SInt8	m_ownerOffset;
	SInt8	m_objectQualityType;
	UInt32	m_collisionFilterInfo;
};

ASSERT_SIZE(hkpTypedBroadPhaseHandle, 0xC);
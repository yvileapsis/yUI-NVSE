#pragma once

#include "NiPSysFieldModifier.hpp"

NiSmartPointer(NiPSysAirFieldModifier);

class NiPSysAirFieldModifier : public NiPSysFieldModifier {
public:
	NiPSysAirFieldModifier();
	virtual ~NiPSysAirFieldModifier();

	NiPoint3	m_kLastFieldPosition;
	float		m_fLastUpdateTime;
	NiPoint3	m_kDirection;
	NiPoint3	m_kUnitDirection;
	float		m_fAirFriction;
	float		m_fInheritVelocity;
	bool		m_bInheritRotation;
	bool		m_bComponentOnly;
	bool		m_bEnableSpread;
	float		m_fSpread;

	CREATE_OBJECT(NiPSysAirFieldModifier, 0xC2F630)
};

ASSERT_SIZE(NiPSysAirFieldModifier, 0x68);
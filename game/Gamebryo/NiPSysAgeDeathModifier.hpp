#pragma once

#include "NiPSysModifier.hpp"
#include "NiPSysSpawnModifier.hpp"

NiSmartPointer(NiPSysAgeDeathModifier);

class NiPSysAgeDeathModifier : public NiPSysModifier {
public:
	NiPSysAgeDeathModifier();
	virtual ~NiPSysAgeDeathModifier();

	bool					m_bSpawnOnDeath;
	NiPSysSpawnModifier*	m_pkSpawnModifier;

	CREATE_OBJECT(NiPSysAgeDeathModifier, 0xC2EF20)
};

ASSERT_SIZE(NiPSysAgeDeathModifier, 0x20);
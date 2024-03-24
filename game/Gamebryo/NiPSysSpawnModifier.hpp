#pragma once

#include "NiPSysModifier.hpp"

NiSmartPointer(NiPSysSpawnModifier);

class NiPSysSpawnModifier : public NiPSysModifier {
public:
	NiPSysSpawnModifier();
	virtual ~NiPSysSpawnModifier();

	UInt16		m_usNumSpawnGenerations;
	float		m_fPercentageSpawned;
	UInt16		m_usMinNumToSpawn;
	UInt16		m_usMaxNumToSpawn;
	float		m_fSpawnSpeedChaos;
	float		m_fSpawnDirChaos;
	float		m_fLifeSpan;
	float		m_fLifeSpanVar;

	CREATE_OBJECT(NiPSysSpawnModifier, 0xC23B20)
};

ASSERT_SIZE(NiPSysSpawnModifier, 0x34);
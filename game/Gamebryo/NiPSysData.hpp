#pragma once

#include "NiParticlesData.hpp"
#include "NiParticleInfo.hpp"

NiSmartPointer(NiPSysData);

class NiPSysData : public NiParticlesData {
public:
	NiPSysData();
	virtual ~NiPSysData();
	virtual UInt16	AddParticle();
	virtual void	ResolveAddedParticles();

	NiParticleInfo*		m_pkParticleInfo;
	float*				m_pfRotationSpeeds;
	UInt16				m_usNumAddedParticles;
	UInt16				m_usAddedParticlesBase;

	CREATE_OBJECT(NiPSysData, 0xC24DA0);

	UInt16 GetNumParticles();
	NiParticleInfo* GetParticleInfo();
};

ASSERT_SIZE(NiPSysData, 0x7C);
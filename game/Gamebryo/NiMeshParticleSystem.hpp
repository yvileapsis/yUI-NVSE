#pragma once

#include "NiParticleSystem.hpp"

NiSmartPointer(NiMeshParticleSystem);

class NiMeshParticleSystem : public NiParticleSystem {
public:
	NiMeshParticleSystem();
	virtual ~NiMeshParticleSystem();

	float	m_fTime;
	bool	m_bUpdateControllers;

	CREATE_OBJECT(NiMeshParticleSystem, 0xC21E90);
};

ASSERT_SIZE(NiMeshParticleSystem, 0x118);
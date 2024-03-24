#pragma once

#include "NiParticles.hpp"

NiSmartPointer(NiParticleMeshes);

class NiParticleMeshes : public NiParticles {
public:
	NiParticleMeshes();
	virtual ~NiParticleMeshes();

	float	m_fTime;
	bool	m_bUpdateControllers;

	CREATE_OBJECT(NiParticleMeshes, 0xA98E70);
};

ASSERT_SIZE(NiParticleMeshes, 0xCC)
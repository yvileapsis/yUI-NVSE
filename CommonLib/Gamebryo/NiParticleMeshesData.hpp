#pragma once

#include "NiParticlesData.hpp"
#include "NiNode.hpp"

NiSmartPointer(NiParticleMeshesData);

class NiParticleMeshesData : public NiParticlesData {
public:
	NiParticleMeshesData();
	virtual ~NiParticleMeshesData();

	NiNodePtr	m_spParticleMeshes;
	bool		m_bNeedsPropertyEffectUpdate;

	CREATE_OBJECT(NiParticleMeshesData, 0xA98D30)
};

ASSERT_SIZE(NiParticleMeshesData, 0x78)
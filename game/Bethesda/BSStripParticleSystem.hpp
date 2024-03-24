#pragma once

#include "NiParticleSystem.hpp"

NiSmartPointer(BSStripParticleSystem);

class BSStripParticleSystem : public NiParticleSystem {
public:
	BSStripParticleSystem();
	~BSStripParticleSystem();

	CREATE_OBJECT(BSStripParticleSystem, 0xC25820);
	NIRTTI_ADDRESS(0x1202560);
};

ASSERT_SIZE(BSStripParticleSystem, 0x110);
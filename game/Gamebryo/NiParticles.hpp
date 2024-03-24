#pragma once

#include "NiGeometry.hpp"

NiSmartPointer(NiParticles);

class NiParticles : public NiGeometry {
public:
	NiParticles();
	virtual ~NiParticles();

	CREATE_OBJECT(NiParticles, 0xA9B150)
};

ASSERT_SIZE(NiParticles, 0xC4)
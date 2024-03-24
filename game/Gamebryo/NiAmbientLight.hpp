#pragma once

#include "NiLight.hpp"

NiSmartPointer(NiAmbientLight);

class NiAmbientLight : public NiLight {
public:
	NiAmbientLight();
	virtual ~NiAmbientLight();

	CREATE_OBJECT(NiAmbientLight, 0xA9B330)
};

ASSERT_SIZE(NiAmbientLight, 0xF0)
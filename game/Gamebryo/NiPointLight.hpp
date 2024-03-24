#pragma once

#include "NiLight.hpp"

NiSmartPointer(NiPointLight);

class NiPointLight : public NiLight {
public:
	NiPointLight();
	virtual ~NiPointLight();

	// Bethesda uses these fields for position offset (for flickering lights)
	union {
		struct {
			float m_fAtten0;
			float m_fAtten1;
			float m_fAtten2;
		};
		NiPoint3 m_kLightOffset;
	};

	CREATE_OBJECT(NiPointLight, 0xA7D6E0);
	NIRTTI_ADDRESS(0x11F4A98);
};

ASSERT_SIZE(NiPointLight, 0xFC)
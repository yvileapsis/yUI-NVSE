#pragma once

#include "NiPSysModifier.hpp"
#include "NiColorData.hpp"

NiSmartPointer(NiPSysColorModifier);

class NiPSysColorModifier : public NiPSysModifier {
public:
	NiPSysColorModifier();
	virtual ~NiPSysColorModifier();

	NiColorDataPtr	m_spColorData;
	float			m_fLoKeyTime;
	float			m_fHiKeyTime;

	CREATE_OBJECT(NiPSysColorModifier, 0xC2D180)
};

ASSERT_SIZE(NiPSysColorModifier, 0x24);
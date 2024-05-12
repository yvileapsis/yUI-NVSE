#pragma once

#include "NiPSysFieldModifier.hpp"

NiSmartPointer(NiPSysTurbulenceFieldModifier);

class NiPSysTurbulenceFieldModifier : public NiPSysFieldModifier {
public:
	NiPSysTurbulenceFieldModifier();
	virtual ~NiPSysTurbulenceFieldModifier();

	float m_fFrequency;
	float m_fFrequencyUpdateTime;
	float m_fLastUpdateTime;

	CREATE_OBJECT(NiPSysTurbulenceFieldModifier, 0xC27350)
};

ASSERT_SIZE(NiPSysTurbulenceFieldModifier, 0x3C);
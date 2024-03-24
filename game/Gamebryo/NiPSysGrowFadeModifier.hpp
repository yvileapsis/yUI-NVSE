#pragma once

#include "NiPSysModifier.hpp"

NiSmartPointer(NiPSysGrowFadeModifier);

class NiPSysGrowFadeModifier : public NiPSysModifier {
public:
	NiPSysGrowFadeModifier();
	virtual ~NiPSysGrowFadeModifier();

	float	m_fGrowTime;
	UInt16	m_usGrowGeneration;
	float	m_fFadeTime;
	UInt16	m_usFadeGeneration;
	float	m_fBaseScale;

	CREATE_OBJECT(NiPSysGrowFadeModifier, 0xC2A2B0)
};

ASSERT_SIZE(NiPSysGrowFadeModifier, 0x2C);
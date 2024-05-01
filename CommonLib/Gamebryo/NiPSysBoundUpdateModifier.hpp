#pragma once

#include "NiPSysModifier.hpp"
#include "NiBound.hpp"

NiSmartPointer(NiPSysBoundUpdateModifier);

class NiPSysBoundUpdateModifier : public NiPSysModifier {
public:
	NiPSysBoundUpdateModifier();
	virtual ~NiPSysBoundUpdateModifier();

	SInt16		m_sUpdateSkip;
	UInt16		m_usUpdateCount;
	NiBound*	m_pkSkipBounds;

	CREATE_OBJECT(NiPSysBoundUpdateModifier, 0xC2D690)
};

ASSERT_SIZE(NiPSysBoundUpdateModifier, 0x20);
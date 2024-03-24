#pragma once

#include "NiPSysModifierCtlr.hpp"
#include "NiTSet.hpp"

NiSmartPointer(NiPSysEmitterCtlr);

class NiPSysEmitterCtlr : public NiPSysModifierCtlr {
public:
	NiPSysEmitterCtlr();
	virtual ~NiPSysEmitterCtlr();

	NiInterpolatorPtr	m_spEmitterActiveInterpolator;
	NiInterpolator*		m_pkLastBirthRateInterpolator;
	float				m_fLastScaledTime;
	bool				m_bLastActive;
	NiFloatSet			m_kParticleAges;

	CREATE_OBJECT(NiPSysEmitterCtlr, 0xC1C5E0)
};

ASSERT_SIZE(NiPSysEmitterCtlr, 0x5C)
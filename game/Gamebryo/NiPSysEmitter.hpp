#pragma once

#include "NiPSysModifier.hpp"
#include "NiColor.hpp"

NiSmartPointer(NiPSysEmitter);

class NiPSysEmitter : public NiPSysModifier {
public:
	NiPSysEmitter();
	virtual ~NiPSysEmitter();

	float		m_fSpeed;
	float		m_fSpeedVar;
	float		m_fDeclination;
	float		m_fDeclinationVar;
	float		m_fPlanarAngle;
	float		m_fPlanarAngleVar;
	NiColorA	m_kInitialColor;
	float		m_fInitialRadius;
	float		m_fRadiusVar;
	float		m_fLifeSpan;
	float		m_fLifeSpanVar;
	float		m_fScale;
};

ASSERT_SIZE(NiPSysEmitter, 0x54)
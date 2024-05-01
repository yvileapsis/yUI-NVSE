#pragma once

#include "NiPSysModifier.hpp"
#include "NiAVObject.hpp"
#include "NiPoint3.hpp"

NiSmartPointer(NiPSysGravityModifier);

class NiPSysGravityModifier : public NiPSysModifier {
public:
	NiPSysGravityModifier();
	virtual ~NiPSysGravityModifier();

	enum ForceType {
		FORCE_PLANAR,
		FORCE_SPHERICAL
	};

	NiAVObject*		m_pkGravityObj;
	NiPoint3		m_kGravityAxis;
	float			m_fDecay;
	float			m_fStrength;
	ForceType		m_eType;
	float			m_fTurbulence;
	float			m_fScale;
	bool			m_bWorldAligned;

	CREATE_OBJECT(NiPSysGravityModifier, 0xC22AD0)
};

ASSERT_SIZE(NiPSysGravityModifier, 0x40);
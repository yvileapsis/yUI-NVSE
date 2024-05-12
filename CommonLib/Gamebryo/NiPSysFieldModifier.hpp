#pragma once

#include "NiPSysModifier.hpp"
#include "NiAVObject.hpp"

NiSmartPointer(NiPSysFieldModifier);

class NiPSysFieldModifier : public NiPSysModifier {
public:
	NiPSysFieldModifier();
	virtual ~NiPSysFieldModifier();

	NiAVObject*		m_pkFieldObj;
	float			m_fMagnitude;
	float			m_fAttenuation;
	bool			m_bUseMaxDistance;
	float			m_fMaxDistance;
	float			m_fMaxDistanceSqr;
};

ASSERT_SIZE(NiPSysFieldModifier, 0x30);
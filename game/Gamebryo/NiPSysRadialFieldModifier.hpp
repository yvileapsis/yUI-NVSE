#pragma once

#include "NiPSysFieldModifier.hpp"

NiSmartPointer(NiPSysRadialFieldModifier);

class NiPSysRadialFieldModifier : public NiPSysFieldModifier {
public:
	NiPSysRadialFieldModifier();
	virtual ~NiPSysRadialFieldModifier();

	float m_fRadialType;

	CREATE_OBJECT(NiPSysRadialFieldModifier, 0xC28570)
};

ASSERT_SIZE(NiPSysRadialFieldModifier, 0x34);
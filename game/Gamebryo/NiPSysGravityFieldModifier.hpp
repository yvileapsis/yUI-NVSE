#pragma once

#include "NiPSysFieldModifier.hpp"

NiSmartPointer(NiPSysGravityFieldModifier);

class NiPSysGravityFieldModifier : public NiPSysFieldModifier {
public:
	NiPSysGravityFieldModifier();
	virtual ~NiPSysGravityFieldModifier();

	NiPoint3 m_kDirection;
	NiPoint3 m_kUnitDirection;

	CREATE_OBJECT(NiPSysGravityFieldModifier, 0xC2AA50)
};

ASSERT_SIZE(NiPSysGravityFieldModifier, 0x48);
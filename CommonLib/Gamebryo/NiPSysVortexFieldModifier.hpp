#pragma once

#include "NiPSysFieldModifier.hpp"

NiSmartPointer(NiPSysVortexFieldModifier);

class NiPSysVortexFieldModifier : public NiPSysFieldModifier {
public:
	NiPSysVortexFieldModifier();
	virtual ~NiPSysVortexFieldModifier();

	NiPoint3 m_kDirection;

	CREATE_OBJECT(NiPSysVortexFieldModifier, 0xC26790)
};

ASSERT_SIZE(NiPSysVortexFieldModifier, 0x3C);
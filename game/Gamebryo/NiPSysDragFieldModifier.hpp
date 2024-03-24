#pragma once

#include "NiPSysFieldModifier.hpp"

NiSmartPointer(NiPSysDragFieldModifier);

class NiPSysDragFieldModifier : public NiPSysFieldModifier {
public:
	NiPSysDragFieldModifier();
	virtual ~NiPSysDragFieldModifier();

	bool		m_bUseDirection;
	NiPoint3	m_kDirection;

	CREATE_OBJECT(NiPSysDragFieldModifier, 0xC2C7B0)
};

ASSERT_SIZE(NiPSysDragFieldModifier, 0x40);
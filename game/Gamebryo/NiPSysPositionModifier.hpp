#pragma once

#include "NiPSysModifier.hpp"

NiSmartPointer(NiPSysPositionModifier);

class NiPSysPositionModifier : public NiPSysModifier {
public:
	NiPSysPositionModifier();
	virtual ~NiPSysPositionModifier();

	CREATE_OBJECT(NiPSysPositionModifier, 0xC28B20)
};

ASSERT_SIZE(NiPSysPositionModifier, 0x18);
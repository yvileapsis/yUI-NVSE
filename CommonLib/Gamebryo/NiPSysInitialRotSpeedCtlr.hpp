#pragma once

#include "NiPSysModifierFloatCtlr.hpp"

NiSmartPointer(NiPSysInitialRotSpeedCtlr);

class NiPSysInitialRotSpeedCtlr : public NiPSysModifierFloatCtlr {
public:
	NiPSysInitialRotSpeedCtlr();
	virtual ~NiPSysInitialRotSpeedCtlr();

	CREATE_OBJECT(NiPSysInitialRotSpeedCtlr, 0xC29F50)
};

ASSERT_SIZE(NiPSysInitialRotSpeedCtlr, 0x40);
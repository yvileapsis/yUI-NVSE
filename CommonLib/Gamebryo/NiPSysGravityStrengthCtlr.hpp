#pragma once

#include "NiPSysModifierFloatCtlr.hpp"

NiSmartPointer(NiPSysGravityStrengthCtlr);

class NiPSysGravityStrengthCtlr : public NiPSysModifierFloatCtlr {
public:
	NiPSysGravityStrengthCtlr();
	virtual ~NiPSysGravityStrengthCtlr();

	CREATE_OBJECT(NiPSysGravityStrengthCtlr, 0xC2A870)
};

ASSERT_SIZE(NiPSysGravityStrengthCtlr, 0x40);
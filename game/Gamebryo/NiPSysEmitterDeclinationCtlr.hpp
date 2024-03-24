#pragma once

#include "NiPSysModifierFloatCtlr.hpp"

NiSmartPointer(NiPSysEmitterDeclinationCtlr);

class NiPSysEmitterDeclinationCtlr : public NiPSysModifierFloatCtlr {
public:
	NiPSysEmitterDeclinationCtlr();
	virtual ~NiPSysEmitterDeclinationCtlr();

	CREATE_OBJECT(NiPSysEmitterDeclinationCtlr, 0xC2B970)
};

ASSERT_SIZE(NiPSysEmitterDeclinationCtlr, 0x40);
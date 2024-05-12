#pragma once

#include "NiPSysModifierFloatCtlr.hpp"

NiSmartPointer(NiPSysEmitterSpeedCtlr);

class NiPSysEmitterSpeedCtlr : public NiPSysModifierFloatCtlr {
public:
	NiPSysEmitterSpeedCtlr();
	virtual ~NiPSysEmitterSpeedCtlr();

	CREATE_OBJECT(NiPSysEmitterSpeedCtlr, 0xC2B330)
};

ASSERT_SIZE(NiPSysEmitterSpeedCtlr, 0x40);
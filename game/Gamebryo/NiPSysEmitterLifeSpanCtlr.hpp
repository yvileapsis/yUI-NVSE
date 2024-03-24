#pragma once

#include "NiPSysModifierFloatCtlr.hpp"

NiSmartPointer(NiPSysEmitterLifeSpanCtlr);

class NiPSysEmitterLifeSpanCtlr : public NiPSysModifierFloatCtlr {
public:
	NiPSysEmitterLifeSpanCtlr();
	virtual ~NiPSysEmitterLifeSpanCtlr();

	CREATE_OBJECT(NiPSysEmitterLifeSpanCtlr, 0xC2B620)
};

ASSERT_SIZE(NiPSysEmitterLifeSpanCtlr, 0x40);
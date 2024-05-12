#pragma once

#include "NiPSysModifierFloatCtlr.hpp"

NiSmartPointer(NiPSysEmitterInitialRadiusCtlr);

class NiPSysEmitterInitialRadiusCtlr : public NiPSysModifierFloatCtlr {
public:
	NiPSysEmitterInitialRadiusCtlr();
	virtual ~NiPSysEmitterInitialRadiusCtlr();

	CREATE_OBJECT(NiPSysEmitterInitialRadiusCtlr, 0xC2B720)
};

ASSERT_SIZE(NiPSysEmitterInitialRadiusCtlr, 0x40);
#pragma once

#include "NiPSysModifierFloatCtlr.hpp"

NiSmartPointer(NiPSysEmitterDeclinationVarCtlr);

class NiPSysEmitterDeclinationVarCtlr : public NiPSysModifierFloatCtlr {
public:
	NiPSysEmitterDeclinationVarCtlr();
	virtual ~NiPSysEmitterDeclinationVarCtlr();

	CREATE_OBJECT(NiPSysEmitterDeclinationVarCtlr, 0xC2B870)
};

ASSERT_SIZE(NiPSysEmitterDeclinationVarCtlr, 0x40);
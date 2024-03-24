#pragma once

#include "NiPSysModifierFloatCtlr.hpp"

NiSmartPointer(NiPSysEmitterPlanarAngleVarCtlr);

class NiPSysEmitterPlanarAngleVarCtlr : public NiPSysModifierFloatCtlr {
public:
	NiPSysEmitterPlanarAngleVarCtlr();
	virtual ~NiPSysEmitterPlanarAngleVarCtlr();

	CREATE_OBJECT(NiPSysEmitterPlanarAngleVarCtlr, 0xC2B420)
};

ASSERT_SIZE(NiPSysEmitterPlanarAngleVarCtlr, 0x40);
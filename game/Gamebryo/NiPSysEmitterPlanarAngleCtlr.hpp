#pragma once

#include "NiPSysModifierFloatCtlr.hpp"

NiSmartPointer(NiPSysEmitterPlanarAngleCtlr);

class NiPSysEmitterPlanarAngleCtlr : public NiPSysModifierFloatCtlr {
public:
	NiPSysEmitterPlanarAngleCtlr();
	virtual ~NiPSysEmitterPlanarAngleCtlr();

	CREATE_OBJECT(NiPSysEmitterPlanarAngleCtlr, 0xC2B520)
};

ASSERT_SIZE(NiPSysEmitterPlanarAngleCtlr, 0x40);
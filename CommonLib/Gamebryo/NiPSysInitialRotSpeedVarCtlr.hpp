#pragma once

#include "NiPSysModifierFloatCtlr.hpp"

NiSmartPointer(NiPSysInitialRotSpeedVarCtlr);

class NiPSysInitialRotSpeedVarCtlr : public NiPSysModifierFloatCtlr {
public:
	NiPSysInitialRotSpeedVarCtlr();
	virtual ~NiPSysInitialRotSpeedVarCtlr();

	CREATE_OBJECT(NiPSysInitialRotSpeedVarCtlr, 0xC29E50)
};

ASSERT_SIZE(NiPSysInitialRotSpeedVarCtlr, 0x40);
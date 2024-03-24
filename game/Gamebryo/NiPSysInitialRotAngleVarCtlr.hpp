#pragma once

#include "NiPSysModifierFloatCtlr.hpp"

NiSmartPointer(NiPSysInitialRotAngleVarCtlr);

class NiPSysInitialRotAngleVarCtlr : public NiPSysModifierFloatCtlr {
public:
	NiPSysInitialRotAngleVarCtlr();
	virtual ~NiPSysInitialRotAngleVarCtlr();

	CREATE_OBJECT(NiPSysInitialRotAngleVarCtlr, 0xC2A050)
};

ASSERT_SIZE(NiPSysInitialRotAngleVarCtlr, 0x40);
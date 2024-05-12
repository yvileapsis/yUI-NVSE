#pragma once

#include "NiPSysModifierFloatCtlr.hpp"

NiSmartPointer(NiPSysInitialRotAngleCtlr);

class NiPSysInitialRotAngleCtlr : public NiPSysModifierFloatCtlr {
public:
	NiPSysInitialRotAngleCtlr();
	virtual ~NiPSysInitialRotAngleCtlr();

	CREATE_OBJECT(NiPSysInitialRotAngleCtlr, 0xC2A160)
};

ASSERT_SIZE(NiPSysInitialRotAngleCtlr, 0x40);
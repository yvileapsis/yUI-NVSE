#pragma once

#include "NiPSysModifierCtlr.hpp"

NiSmartPointer(NiPSysModifierFloatCtlr);

class NiPSysModifierFloatCtlr : public NiPSysModifierCtlr {
public:
	NiPSysModifierFloatCtlr();
	virtual ~NiPSysModifierFloatCtlr();

	CREATE_OBJECT(NiPSysModifierFloatCtlr, 0xDEADBEEF)
};

ASSERT_SIZE(NiPSysModifierFloatCtlr, 0x40);
#pragma once

#include "NiPSysModifierCtlr.hpp"

NiSmartPointer(NiPSysModifierBoolCtlr);

class NiPSysModifierBoolCtlr : public NiPSysModifierCtlr {
public:
	NiPSysModifierBoolCtlr();
	virtual ~NiPSysModifierBoolCtlr();
};

ASSERT_SIZE(NiPSysModifierBoolCtlr, 0x40);
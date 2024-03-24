#pragma once

#include "NiPSysModifierBoolCtlr.hpp"

NiSmartPointer(NiPSysModifierActiveCtlr);

class NiPSysModifierActiveCtlr : public NiPSysModifierBoolCtlr {
public:
	NiPSysModifierActiveCtlr();
	virtual ~NiPSysModifierActiveCtlr();

	CREATE_OBJECT(NiPSysModifierActiveCtlr, 0xC29D00)
};

ASSERT_SIZE(NiPSysModifierActiveCtlr, 0x40);
#pragma once

#include "NiPSysModifierFloatCtlr.hpp"

NiSmartPointer(NiPSysFieldAttenuationCtlr);

class NiPSysFieldAttenuationCtlr : public NiPSysModifierFloatCtlr {
public:
	NiPSysFieldAttenuationCtlr();
	virtual ~NiPSysFieldAttenuationCtlr();

	CREATE_OBJECT(NiPSysFieldAttenuationCtlr, 0xC2B200)
};

ASSERT_SIZE(NiPSysFieldAttenuationCtlr, 0x40);
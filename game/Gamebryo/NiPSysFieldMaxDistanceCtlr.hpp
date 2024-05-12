#pragma once

#include "NiPSysModifierFloatCtlr.hpp"

NiSmartPointer(NiPSysFieldMaxDistanceCtlr);

class NiPSysFieldMaxDistanceCtlr : public NiPSysModifierFloatCtlr {
public:
	NiPSysFieldMaxDistanceCtlr();
	virtual ~NiPSysFieldMaxDistanceCtlr();

	CREATE_OBJECT(NiPSysFieldMaxDistanceCtlr, 0xC2B010)
};

ASSERT_SIZE(NiPSysFieldMaxDistanceCtlr, 0x40);
#pragma once

#include "NiPSysModifierFloatCtlr.hpp"

NiSmartPointer(NiPSysFieldMagnitudeCtlr);

class NiPSysFieldMagnitudeCtlr : public NiPSysModifierFloatCtlr {
public:
	NiPSysFieldMagnitudeCtlr();
	virtual ~NiPSysFieldMagnitudeCtlr();

	CREATE_OBJECT(NiPSysFieldMagnitudeCtlr, 0xC2B110)
};

ASSERT_SIZE(NiPSysFieldMagnitudeCtlr, 0x40);
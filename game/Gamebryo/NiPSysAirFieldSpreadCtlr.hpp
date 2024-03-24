#pragma once

#include "NiPSysModifierFloatCtlr.hpp"

NiSmartPointer(NiPSysAirFieldSpreadCtlr);

class NiPSysAirFieldSpreadCtlr : public NiPSysModifierFloatCtlr {
public:
	NiPSysAirFieldSpreadCtlr();
	virtual ~NiPSysAirFieldSpreadCtlr();

	CREATE_OBJECT(NiPSysAirFieldSpreadCtlr, 0xC2F3A0)
};

ASSERT_SIZE(NiPSysAirFieldSpreadCtlr, 0x40);
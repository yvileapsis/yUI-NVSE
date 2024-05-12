#pragma once

#include "NiPSysModifierFloatCtlr.hpp"

NiSmartPointer(NiPSysAirFieldAirFrictionCtlr);

class NiPSysAirFieldAirFrictionCtlr : public NiPSysModifierFloatCtlr {
public:
	NiPSysAirFieldAirFrictionCtlr();
	virtual ~NiPSysAirFieldAirFrictionCtlr();

	CREATE_OBJECT(NiPSysAirFieldAirFrictionCtlr, 0xC30790)
};

ASSERT_SIZE(NiPSysAirFieldAirFrictionCtlr, 0x40);
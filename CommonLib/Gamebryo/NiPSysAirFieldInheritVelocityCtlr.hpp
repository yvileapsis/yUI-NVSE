#pragma once

#include "NiPSysModifierFloatCtlr.hpp"

NiSmartPointer(NiPSysAirFieldInheritVelocityCtlr);

class NiPSysAirFieldInheritVelocityCtlr : public NiPSysModifierFloatCtlr {
public:
	NiPSysAirFieldInheritVelocityCtlr();
	virtual ~NiPSysAirFieldInheritVelocityCtlr();

	CREATE_OBJECT(NiPSysAirFieldInheritVelocityCtlr, 0xC30600)
};

ASSERT_SIZE(NiPSysAirFieldInheritVelocityCtlr, 0x40);
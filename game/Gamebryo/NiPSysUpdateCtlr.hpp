#pragma once

#include "NiTimeController.hpp"

NiSmartPointer(NiPSysUpdateCtlr);

class NiPSysUpdateCtlr : public NiTimeController {
public:
	NiPSysUpdateCtlr();
	virtual ~NiPSysUpdateCtlr();

	CREATE_OBJECT(NiPSysUpdateCtlr, 0xC270F0)
};

ASSERT_SIZE(NiPSysUpdateCtlr, 0x34);
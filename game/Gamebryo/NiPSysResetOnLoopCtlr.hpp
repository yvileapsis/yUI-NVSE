#pragma once

#include "NiTimeController.hpp"

NiSmartPointer(NiPSysResetOnLoopCtlr);

class NiPSysResetOnLoopCtlr : public NiTimeController {
public:
	NiPSysResetOnLoopCtlr();
	virtual ~NiPSysResetOnLoopCtlr();

	float m_fLastScaledTime;

	CREATE_OBJECT(NiPSysResetOnLoopCtlr, 0xC283B0)
};

ASSERT_SIZE(NiPSysResetOnLoopCtlr, 0x38);
#pragma once

#include "NiSingleInterpController.hpp"
#include "NiFixedString.hpp"

class NiPSysModifier;

NiSmartPointer(NiPSysModifierCtlr);

class NiPSysModifierCtlr : public NiSingleInterpController {
public:
	NiPSysModifierCtlr();
	virtual ~NiPSysModifierCtlr();

	NiFixedString	m_kModifierName;
	NiPSysModifier* m_pkModifier;

	CREATE_OBJECT(NiPSysModifierCtlr, 0xC5C9E0);
};

ASSERT_SIZE(NiPSysModifierCtlr, 0x40)
#pragma once

#include "NiFloatInterpController.hpp"

NiSmartPointer(BSRefractionStrengthController);

class BSRefractionStrengthController : public NiFloatInterpController {
public:
	BSRefractionStrengthController();
	~BSRefractionStrengthController();

	CREATE_OBJECT(BSRefractionStrengthController, 0xBC36A0)
};

ASSERT_SIZE(BSRefractionStrengthController, 0x38);
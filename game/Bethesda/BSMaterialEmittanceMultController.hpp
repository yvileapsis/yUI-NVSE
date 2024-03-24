#pragma once

#include "NiFloatInterpController.hpp"

NiSmartPointer(BSMaterialEmittanceMultController);

class BSMaterialEmittanceMultController : public NiFloatInterpController {
public:
	BSMaterialEmittanceMultController();
	virtual ~BSMaterialEmittanceMultController();

	CREATE_OBJECT(BSMaterialEmittanceMultController, 0xBC3190);
};

ASSERT_SIZE(BSMaterialEmittanceMultController, 0x38);
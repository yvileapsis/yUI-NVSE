#pragma once

#include "NiFloatInterpController.hpp"

NiSmartPointer(BSRefractionFirePeriodController);

class BSRefractionFirePeriodController : public NiFloatInterpController {
public:
	BSRefractionFirePeriodController();
	~BSRefractionFirePeriodController();

	CREATE_OBJECT(BSRefractionFirePeriodController, 0xBC33F0)
};

ASSERT_SIZE(BSRefractionFirePeriodController, 0x38);
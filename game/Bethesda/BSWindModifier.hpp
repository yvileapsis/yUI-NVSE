#pragma once

#include "NiPSysModifier.hpp"

NiSmartPointer(BSWindModifier);

class BSWindModifier : public NiPSysModifier {
public:
	BSWindModifier();
	~BSWindModifier();

	float float18;

	CREATE_OBJECT(BSWindModifier, 0xC46740)
};

ASSERT_SIZE(BSWindModifier, 0x1C)
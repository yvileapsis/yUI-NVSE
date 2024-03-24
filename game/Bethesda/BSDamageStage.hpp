#pragma once

#include "BSRangeNode.hpp"

NiSmartPointer(BSDamageStage);

class BSDamageStage : public BSRangeNode {
public:
	BSDamageStage();
	~BSDamageStage();

	CREATE_OBJECT(BSDamageStage, 0xC4B0C0);
};

ASSERT_SIZE(BSDamageStage, 0xB0)
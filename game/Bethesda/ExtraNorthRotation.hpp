#pragma once

#include "BSExtraData.hpp"

class ExtraNorthRotation : public BSExtraData {
public:
	ExtraNorthRotation();
	virtual ~ExtraNorthRotation();

	float fNorthRot;
};

ASSERT_SIZE(ExtraNorthRotation, 0x10);
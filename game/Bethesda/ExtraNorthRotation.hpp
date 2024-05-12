#pragma once
#include "BSExtraData.hpp"

// 0x10
class ExtraNorthRotation : public BSExtraData {
public:
	ExtraNorthRotation();
	virtual ~ExtraNorthRotation();

	Float32 fNorthRot;
};
static_assert(sizeof(ExtraNorthRotation) == 0x10);
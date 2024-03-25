#pragma once
#include "BSExtraData.hpp"

// 0xC
class ExtraWornLeft : public BSExtraData {
public:
	ExtraWornLeft();
	virtual ~ExtraWornLeft();

	static ExtraWornLeft* Create();
};
static_assert(sizeof(ExtraWornLeft) == 0x0C);
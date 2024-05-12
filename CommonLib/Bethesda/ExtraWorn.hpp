#pragma once
#include "BSExtraData.hpp"

class ExtraWorn : public BSExtraData {
public:
	ExtraWorn();
	virtual ~ExtraWorn();

	static ExtraWorn* Create();
};
static_assert(sizeof(ExtraWorn) == 0x0C);
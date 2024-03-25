#pragma once
#include "BSExtraData.hpp"

class ExtraHealth : public BSExtraData {
public:
	ExtraHealth();
	virtual ~ExtraHealth();
	
	Float32 health;

	static ExtraHealth* Create();
};
static_assert(sizeof(ExtraHealth) == 0x10);
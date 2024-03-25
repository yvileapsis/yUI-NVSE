#pragma once
#include "BSExtraData.hpp"

// 0x10
class ExtraObjectHealth : public BSExtraData {
public:
	Float32			health;		// 0C
	static ExtraObjectHealth* __stdcall Create(float _health);
};
static_assert(sizeof(ExtraObjectHealth) == 0x10);
#pragma once
#include "TESObjectMISC.hpp"

// 0xAC
class TESKey : public TESObjectMISC {
public:
	TESKey();
	~TESKey();
};
static_assert(sizeof(TESKey) == 0xAC);
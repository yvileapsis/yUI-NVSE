#pragma once

#include "TESObjectMISC.hpp"

class TESKey : public TESObjectMISC {
public:
	TESKey();
	~TESKey();
};

ASSERT_SIZE(TESKey, 0xAC);
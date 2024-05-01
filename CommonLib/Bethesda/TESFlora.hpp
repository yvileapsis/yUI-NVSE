#pragma once

#include "TESProduceForm.hpp"
#include "TESObjectACTI.hpp"

class TESFlora : public TESProduceForm {
public:
	TESObjectACTI tesobjectactiC;
};

ASSERT_SIZE(TESFlora, 0x9C);
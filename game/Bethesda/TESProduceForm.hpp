#pragma once

#include "BaseFormComponent.hpp"

class TESProduceForm : public BaseFormComponent {
public:
	int unk4;
	char unk8[4];
};

ASSERT_SIZE(TESProduceForm, 0xC);
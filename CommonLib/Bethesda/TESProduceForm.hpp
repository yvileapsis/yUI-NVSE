#pragma once
#include "BaseFormComponent.hpp"

class TESProduceForm : public BaseFormComponent
{
public:
	int unk4;
	char unk8[4];
};
static_assert(sizeof(TESProduceForm) == 0xC);
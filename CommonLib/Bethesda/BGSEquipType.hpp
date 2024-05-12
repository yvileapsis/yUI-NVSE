#pragma once
#include "BaseFormComponent.hpp"

// 0x8
class BGSEquipType : public BaseFormComponent
{
public:
	BGSEquipType();
	~BGSEquipType();

	UInt32	uiEquipType;
};
static_assert(sizeof(BGSEquipType) == 0x8);
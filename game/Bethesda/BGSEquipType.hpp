#pragma once

#include "BaseFormComponent.hpp"

class BGSEquipType : public BaseFormComponent {
public:
	BGSEquipType();
	~BGSEquipType();

	UInt32	equipType;	// 08
};
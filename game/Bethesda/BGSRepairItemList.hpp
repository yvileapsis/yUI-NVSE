#pragma once

#include "BaseFormComponent.hpp"

class BGSListForm;

class BGSRepairItemList : public BaseFormComponent {
public:
	BGSRepairItemList();
	~BGSRepairItemList();

	BGSListForm* listForm;	// 04
};
#pragma once
#include "BaseFormComponent.hpp"

class BGSListForm;

// 0x8
class BGSRepairItemList : public BaseFormComponent {
public:
	BGSRepairItemList();
	~BGSRepairItemList();

	BGSListForm* pkRepairList;	// 04
};
static_assert(sizeof(BGSRepairItemList) == 0x8);
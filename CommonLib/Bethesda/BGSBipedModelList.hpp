#pragma once
#include "BaseFormComponent.hpp"

class BGSListForm;

// 0x8
class BGSBipedModelList : public BaseFormComponent
{
public:
	BGSBipedModelList();
	~BGSBipedModelList();

	BGSListForm* pkModels;		// 004
};
static_assert(sizeof(BGSBipedModelList) == 0x8);
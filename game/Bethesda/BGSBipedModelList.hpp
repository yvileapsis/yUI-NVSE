#pragma once

#include "BaseFormComponent.hpp"

class BGSListForm;

class BGSBipedModelList : public BaseFormComponent {
public:
	BGSBipedModelList();
	~BGSBipedModelList();

	BGSListForm* models;		// 004
	// 008
};
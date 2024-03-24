#pragma once

#include "BaseFormComponent.hpp"

class TESWeightForm : public BaseFormComponent {
public:
	TESWeightForm();
	~TESWeightForm();

	float	fWeight;		// 004
	// 008
};

ASSERT_SIZE(TESWeightForm, 0x8);
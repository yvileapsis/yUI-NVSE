#pragma once
#include "BaseFormComponent.hpp"
// Complete

// 0x8
class TESWeightForm : public BaseFormComponent {
public:
	TESWeightForm();
	~TESWeightForm();

	Float32		fWeight;		// 004
	// 008
};
static_assert(sizeof(TESWeightForm) == 0x8);
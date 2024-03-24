#pragma once

#include "TESForm.hpp"

class TESRecipeCategory : public TESForm {
public:
	TESRecipeCategory();
	~TESRecipeCategory();

	TESFullName			fullName;	// 18
	UInt32				flags;		// 24
};

ASSERT_SIZE(TESRecipeCategory, 0x28);
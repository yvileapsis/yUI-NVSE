#pragma once
#include "TESForm.hpp"

// 0x28
class TESRecipeCategory : public TESForm, public TESFullName 
{
public:
	TESRecipeCategory();
	~TESRecipeCategory();

	UInt32				flags;		// 24
};
static_assert(sizeof(TESRecipeCategory) == 0x28);
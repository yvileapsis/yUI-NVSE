#pragma once
#include "BaseFormComponent.hpp"
#include "BSSimpleList.hpp"

// 0xC
class TESAnimation : public BaseFormComponent {
public:
	TESAnimation();
	~TESAnimation();

	// constructor and Fn_01 sugest this is a BSSimpleList of char string.
	// In GECK it is an array
	BSSimpleList<char> animNames;
};
static_assert(sizeof(TESAnimation) == 0xC);
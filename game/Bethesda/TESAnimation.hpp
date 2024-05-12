#pragma once

#include "BaseFormComponent.hpp"
#include "BSSimpleList.hpp"

class TESAnimation : public BaseFormComponent {
public:
	TESAnimation();
	~TESAnimation();

	typedef BSSimpleList<char> AnimNames;

	// constructor and Fn_01 sugest this is a BSSimpleList of char string.
	// In GECK it is an array
	AnimNames	animNames;
	// 00C
};

ASSERT_SIZE(TESAnimation, 0xC);
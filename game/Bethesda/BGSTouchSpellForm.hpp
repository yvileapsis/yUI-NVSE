#pragma once

#include "BaseFormComponent.hpp"

class BGSTouchSpellForm : public BaseFormComponent {
public:
	BGSTouchSpellForm();
	~BGSTouchSpellForm();

	TESForm* unarmedEffect;	// 04
	UInt16	unarmedAnim;	// 08
	UInt16	pad0A;		// 0A
};

ASSERT_SIZE(BGSTouchSpellForm, 0xC);
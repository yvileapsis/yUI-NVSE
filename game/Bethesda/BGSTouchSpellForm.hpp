#pragma once
#include "BaseFormComponent.hpp"

// 0xC
class BGSTouchSpellForm : public BaseFormComponent {
public:
	BGSTouchSpellForm();
	~BGSTouchSpellForm();

	MagicItemForm*	pkUnarmedEffect;	// 04
	UInt16			pkUnarmedAnim;		// 08
	UInt16			pad0A;				// 0A
};
static_assert(sizeof(BGSTouchSpellForm) == 0xC);
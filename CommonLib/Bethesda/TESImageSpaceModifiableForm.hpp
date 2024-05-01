#pragma once
#include "BaseFormComponent.hpp"

// 0x8
class TESImageSpaceModifiableForm : public BaseFormComponent {
public:
	TESImageSpaceModifiableForm();
	~TESImageSpaceModifiableForm();

	UInt32	unk04;	// 04
};
static_assert(sizeof(TESImageSpaceModifiableForm) == 0x8);
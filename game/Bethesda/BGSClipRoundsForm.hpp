#pragma once
#include "BaseFormComponent.hpp"

// 0x8
class BGSClipRoundsForm : public BaseFormComponent {
public:
	BGSClipRoundsForm();
	~BGSClipRoundsForm();

	UInt8	ucClipRounds;
	UInt8	pad05[3];
};
static_assert(sizeof(BGSClipRoundsForm) == 0x8);
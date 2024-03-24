#pragma once

#include "BaseFormComponent.hpp"

class BGSClipRoundsForm : public BaseFormComponent {
public:
	BGSClipRoundsForm();
	~BGSClipRoundsForm();

	UInt8	ucClipRounds;
};

ASSERT_SIZE(BGSClipRoundsForm, 0x8);
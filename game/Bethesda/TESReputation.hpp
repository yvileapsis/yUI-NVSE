#pragma once

#include "TESForm.hpp"
#include "TESFullName.hpp"
#include "TESIcon.hpp"
#include "BGSMessageIcon.hpp"

class TESReputation : public TESForm {
	TESFullName name;
	TESIcon icon;
	BGSMessageIcon messageIcon;
	float maxReputation;
	float positiveReputation;
	float negativeReputation;
	UInt32 reputationChangedWasPositive;
};

ASSERT_SIZE(TESReputation, 0x50);
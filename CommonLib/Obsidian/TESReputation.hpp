#pragma once
#include "TESForm.hpp"
#include "TESFullName.hpp"
#include "TESIcon.hpp"
#include "BGSMessageIcon.hpp"

// 0x050
class TESReputation :
	public TESForm,
	public TESFullName,
	public TESIcon,
	public BGSMessageIcon
{
	Float32	fMaxReputation;
	Float32	fPositiveReputation;
	Float32	fNegativeReputation;
	UInt32	uiReputationChangedWasPositive;
};
static_assert(sizeof(TESReputation) == 0x50);
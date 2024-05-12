#pragma once
#include "TESForm.hpp"

// 0x24
class BGSVoiceType : public TESForm {
public:
	BGSVoiceType();
	~BGSVoiceType();

	UInt32		unk018;		// 018
	UInt32		unk01C;		// 01C
	UInt32		unk020;		// 020
};
static_assert(sizeof(BGSVoiceType) == 0x24);
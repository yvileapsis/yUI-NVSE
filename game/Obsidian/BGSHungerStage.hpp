#pragma once
#include "TESForm.hpp"

// 0x20
class BGSHungerStage : public TESForm
{
	UInt32 unk18;
	UInt32 unk1C;
};
static_assert(sizeof(BGSHungerStage) == 0x20);
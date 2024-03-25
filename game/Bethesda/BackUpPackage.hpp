#pragma once
#include "TESPackage.hpp"

// 0x8C
class BackUpPackage : public TESPackage
{
	UInt32 unk80;
	UInt32 unk84;
	UInt32 unk88;
};
static_assert(sizeof(BackUpPackage) == 0x8C);
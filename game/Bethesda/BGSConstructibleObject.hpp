#pragma once
#include "TESObjectMISC.hpp"

// 0xB4
class BGSConstructibleObject : public TESObjectMISC 
{
	UInt32 unkAC;
	UInt32 unkB0;
};
static_assert(sizeof(BGSConstructibleObject) == 0xB4);
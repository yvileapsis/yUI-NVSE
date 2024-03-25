#pragma once

#include "BGSDestructibleObjectForm.hpp"
#include "TESFullName.hpp"
#include "TESObjectSTAT.hpp"

// 0x74
class BGSMovableStatic :
	public TESFullName,					// 00
	public BGSDestructibleObjectForm,	// 14
	public TESObjectSTAT				// 6C
{
public:
	UInt32 gap6C[2];
};
static_assert(sizeof(BGSMovableStatic) == 0x74);
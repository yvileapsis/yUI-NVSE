#pragma once
#include "BSExtraData.hpp"
#include "MagicTarget.hpp"

// 0x28
class NonActorMagicTarget :
	public BSExtraData,			// 0x00
	public MagicTarget			// 0x0C
{
public:
	TESObjectREFR*			targetObject;
	BSSimpleList<UInt32>	unk1;				// 0x20
};
static_assert(sizeof(NonActorMagicTarget) == 0x28);
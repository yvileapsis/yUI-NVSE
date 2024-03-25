#pragma once
#include "BSExtraData.hpp"
#include "MagicCaster.hpp"

// 0x24
class NonActorMagicCaster :
	public BSExtraData,			// 0x0
	public MagicCaster			// 0xC
{
public:
	UInt32			unk0;		// 0x18
	TESObjectREFR*	unk1;		// 0x1C
	TESObjectREFR*	object;		// 0x20
};
static_assert(sizeof(NonActorMagicCaster) == 0x24);
#pragma once
#include "TESBoundObject.hpp"
#include "TESModelTextureSwap.hpp"
#include "BGSMessageIcon.hpp"
#include "TESValueForm.hpp"
#include "BGSDestructibleObjectForm.hpp"
#include "BGSPickupPutdownSounds.hpp"

// 0x9C
class TESCasinoChips :
	public TESBoundObject,
	public TESFullName,
	public TESModelTextureSwap,
	public TESIcon,
	public BGSMessageIcon,
	public TESValueForm,
	public BGSDestructibleObjectForm,
	public BGSPickupPutdownSounds
{
	BSStringT	unk94;
};
static_assert(sizeof(TESCasinoChips) == 0x9C);
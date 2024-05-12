#pragma once
#include "TESBoundObject.hpp"
#include "TESModelTextureSwap.hpp"
#include "BGSMessageIcon.hpp"
#include "TESValueForm.hpp"
#include "TESScriptableForm.hpp"
#include "BGSPickupPutdownSounds.hpp"

// 0xBC
class TESCaravanCard :
	public TESBoundObject,
	public TESFullName,
	public TESModelTextureSwap,
	public TESIcon,
	public BGSMessageIcon,
	public TESValueForm,
	public TESScriptableForm,
	public BGSPickupPutdownSounds
{
public:
	TESCaravanCard();
	~TESCaravanCard();
	UInt32		unk98;
	TESTexture	unk9C;
	TESTexture	unkA8;
	UInt32		unkB4;
	UInt32		unkB8;
};
static_assert(sizeof(TESCaravanCard) == 0xBC);
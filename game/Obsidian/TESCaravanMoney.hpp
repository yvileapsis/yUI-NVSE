#pragma once
#include "TESBoundObject.hpp"
#include "TESModelTextureSwap.hpp"
#include "BGSMessageIcon.hpp"
#include "TESValueForm.hpp"
#include "BGSPickupPutdownSounds.hpp"

// 0xCC
class TESCaravanMoney :
	public TESBoundObject,
	public TESFullName,
	public TESModelTextureSwap,
	public TESIcon,
	public BGSMessageIcon,
	public TESValueForm,
	public BGSPickupPutdownSounds
{
public:
	TESCaravanMoney();
	~TESCaravanMoney();

	TESModelTextureSwap unk8C;
	TESModelTextureSwap unkAC;
};
static_assert(sizeof(TESCaravanMoney) == 0xCC);
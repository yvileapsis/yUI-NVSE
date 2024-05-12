#pragma once
#include "TESBoundObject.hpp"
#include "MagicItem.hpp"
#include "TESModelTextureSwap.hpp"
#include "TESIcon.hpp"
#include "BGSMessageIcon.hpp"
#include "TESScriptableForm.hpp"
#include "TESWeightForm.hpp"
#include "BGSEquipType.hpp"
#include "BGSDestructibleObjectForm.hpp"
#include "BGSPickupPutdownSounds.hpp"

class SpellItem;

// 0xD8
class AlchemyItem :
	public TESBoundObject,				// 00
	public MagicItem,					// 30
	public TESModelTextureSwap,			// 4C
	public TESIcon,						// 6C
	public BGSMessageIcon,				// 78
	public TESScriptableForm,			// 88
	public TESWeightForm,				// 94
	public BGSEquipType,				// 9C
	public BGSDestructibleObjectForm,	// A4
	public BGSPickupPutdownSounds		// AC
{
public:
	AlchemyItem();
	~AlchemyItem();


	UInt32		uiValue;				// B8
	UInt8		ucFlags;				// BC
	UInt8		padBD[3];				// BD
	SpellItem*	pkWithdrawalEffect;		// C0
	Float32		fAddictionChance;		// C4
	TESSound*	pkConsumeSound;			// C8
	TESIcon		kIcon;					// CC

	bool IsPoison();
	bool RestoresAV(int avCode);
	UInt32 HasBaseEffectRestoresAV(SInt32);
	UInt32 HasBaseEffectDamagesAV(SInt32);
	bool IsAddictive();
	bool IsFood();
	bool IsMedicine();
	bool IsFoodAlt();
	bool IsWaterAlt();
};
static_assert(sizeof(AlchemyItem) == 0xD8);
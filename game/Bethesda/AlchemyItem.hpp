#pragma once

#include "BGSDestructibleObjectForm.hpp"
#include "BGSEquipType.hpp"
#include "BGSMessageIcon.hpp"
#include "BGSPickupPutdownSounds.hpp"
#include "EffectItem.hpp"
#include "TESBoundObject.hpp"
#include "TESModelTextureSwap.hpp"
#include "TESScriptableForm.hpp"

class SpellItem;

class AlchemyItem : public TESBoundObject, public TESFullName, public EffectItemList, public TESModelTextureSwap, public TESIcon, 
	public BGSMessageIcon, public TESScriptableForm, public BGSDestructibleObjectForm, public BGSEquipType {
public:
	AlchemyItem();
	~AlchemyItem();

	UInt32						unkA4[2];				// A4
	BGSPickupPutdownSounds		pickupPutdownsounds;	// AC
	UInt32						value;					// B8
	UInt8						alchFlags;				// BC
	UInt8						padBD[3];				// BD
	SpellItem* withdrawalEffect;		// C0
	float						addictionChance;		// C4
	TESSound* consumeSound;			// C8
	UInt32						unkCC[3];				// CC
};

ASSERT_SIZE(AlchemyItem, 0xD8);
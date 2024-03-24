#pragma once

#include "BSSoundHandle.hpp"
#include "BSSimpleList.hpp"

class MagicItem;
class EffectItem;
class MagicTarget;
class MagicCaster;

class ActiveEffect {
public:
	ActiveEffect();
	virtual ~ActiveEffect();
	// Finish vtable

	float					fTimeElapsed;
	MagicItem*				pMagicItem;
	EffectItem*				pEffectItem;
	bool					bApplied;
	bool					bTerminated;
	Bitfield8				ucFlags12;
	bool					bUnk13;
	UInt32					unk14;
	UInt32					uiFlags18;
	float					fMagnitude;
	float					fDuration;
	MagicTarget*			pTarget;
	MagicCaster*			pCaster;
	UInt32					eSpellType;
	BSSoundHandle			hSound;
	TESForm*				pEnchantObject;
	BSSimpleList<TESForm*>*	pHitEffects;
	UInt32					unk44;
};

ASSERT_SIZE(ActiveEffect, 0x48);
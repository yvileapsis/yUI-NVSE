#pragma once

#include "BSTempEffect.hpp"

class ActiveEffect;
class TESObjectREFR;

NiSmartPointer(MagicHitEffect);

class MagicHitEffect : public BSTempEffect {
public:
	MagicHitEffect();
	~MagicHitEffect();

	ActiveEffect*	pActiveEffect;	// 18
	TESObjectREFR*	pTarget;		// 1C
	float			fUnk20;			// 20	Init'd from ActiveEffect.timeElapsed
	Bitfield8		ucFlags;
};
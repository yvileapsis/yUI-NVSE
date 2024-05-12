#pragma once

#include "MobileObject.hpp"
#include "BSSoundHandle.hpp"

class NiLight;
class ActorCause;

class Projectile : public MobileObject {
public:
	BSSimpleList<void>					kImpacts;
	bool								bHasImpacted;
	NiTransform							kFollowOffset;
	UInt32								uiFlags;
	float								fPower;
	float								fSpeedMult;
	float								fRange;
	float								fAge;
	float								fDamage;
	float								fAlpha;
	float								fExplosionTimer;
	float								fBlinkTimer;
	float								flt0EC;
	float								flt0F0;
	float								fWeaponHealthPerc;
	TESObjectWEAP*						pSourceWeapon;
	TESObjectREFR*						pSourceRef;
	TESObjectREFR*						pLiveGrenadeTargetRef;
	NiPoint3							kVector;
	float								fDistTravelled;
	NiPointer<NiLight>					spLight;
	UInt8								byte118;
	NiNode*								node11C;
	NiPointer<ActorCause>				spActorCause;
	float								fDecalSize;
	BSSoundHandle						kPassPlayerSound;
	BSSoundHandle						kCountDownSound;
	void*								unk140;
	ExtraContainerChanges::EntryData*	pRockItLauncherEntry;
	bool								bHasPlayedPassPlayerSound;
	float								fRange;
};

#pragma once
#include "NiTransform.hpp"
#include "MobileObject.hpp"
#include "BSSoundHandle.hpp"

class NiLight;
class ActorCause;
class ItemChange;

class Projectile : public MobileObject
{
public:
	Projectile();
	~Projectile() override;

	BSSimpleList<BGSImpactData*>		kImpacts;				// 0x088
	bool								bHasImpacted;			// 0x08C
	UInt8								pad091[3];
	NiTransform							kFollowOffset;			// 0x094
	UInt32								eFlags;					// 0x0C8
	Float32								fPower;					// 0x0CC
	Float32								fSpeedMult;				// 0x0D0
	Float32								fRange;					// 0x0D4
	Float32								fAge;					// 0x0D8
	Float32								fDamage;				// 0x0DC
	Float32								fAlpha;					// 0x0E0
	Float32								fExplosionTimer;		// 0x0E4
	Float32								fBlinkTimer;			// 0x0E8
	Float32								fSpreadZ;				// 0x0EC
	Float32								fSpreadX;				// 0x0F0
	Float32								fWeaponHealthPerc;		// set but unused
	TESObjectWEAP*						pSourceWeapon;			// 0x0F8
	TESObjectREFR*						pSourceRef;				// 0x0FC
	TESObjectREFR*						pLiveGrenadeTargetRef;	// 0x100 live grenade target
	NiPoint3							kVector;				// 0x104
	UInt32								fDistTravelled;			// 0x110
	NiPointer<NiLight>					spLight;				// 0x114
	UInt8								byte118;				// 0x118
	UInt8								pad119[3];				// 0x119
	NiNode*								node11C;				// 0x11C
	UInt32								unk120;					// 0x120
	Float32								fDecalSize;				// 0x124
	BSSoundHandle						kPassPlayerSound;		// 0x128
	BSSoundHandle						kCountDownSound;		// 0x134
	UInt32								unk140;					// 0x140
	ItemChange*							RockItLauncherItemChange;	// 0x144
	UInt8								bHasPlayedPassPlayerSound;// 0x148
	UInt8								pad149[3];			// 0x149
	Float32								fRange2;
};
static_assert(sizeof(Projectile) == 0x150);
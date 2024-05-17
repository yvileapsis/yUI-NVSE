#pragma once
#include "NiPoint3.hpp"
#include "ActorValue.hpp"

class Actor;
class Projectile;
class Explosion;
class TESObjectWEAP;

// 0x64
class ActorHitData 
{
public:
	enum EnumHitFlags
	{
		kFlag_TargetIsBlocking			= 1 << 0,
		kFlag_TargetWeaponOut			= 1 << 1,
		kFlag_IsCritical				= 1 << 2,
		kFlag_OnDeathCritEffect			= 1 << 3,
		kFlag_IsFatal					= 1 << 4,
		kFlag_DismemberLimb				= 1 << 5,
		kFlag_ExplodeLimb				= 1 << 6,
		kFlag_CrippleLimb				= 1 << 7,
		kFlag_BreakWeaponNonEmbedded	= 1 << 8,
		kFlag_BreakWeaponEmbedded		= 1 << 9,
		kFlag_IsSneakAttack				= 1 << 10,
		kFlag_ArmorPenetrated = 0x80000000	// JIP only
	};

	Actor*				pkSource;		// 00
	Actor*				pkTarget;		// 04
	union								// 08
	{
		Projectile*		pkProjectile;
		Explosion*		pkExplosion;
		Actor*			pkActor;			// TODO:: test again
	};
	EnumActorValueCode	eWeaponActorValue;		// 0C
	SInt32				hitLocation;		// 10
	Float32				fHealthDamage;		// 14
	Float32				fBaseWeaponDamage;	// 18	Skill and weapon condition modifiers included
	Float32				fFatigueDamage;		// 1C
	Float32				fLimbDamage;		// 20
	Float32				fBlockDTMod;		// 24
	Float32				fArmorDamage;		// 28
	Float32				fWeaponDamage;		// 2C
	TESObjectWEAP*		pkWeapon;			// 30
	Float32				fHealthPercent;		// 34
	NiPoint3			kImpactPos;			// 38
	NiPoint3			kIimpactAngle;		// 44
	UInt32				unk50;				// 50
	UInt32				unk54;				// 54
	EnumHitFlags		eFlags;				// 58
	Float32				fDamageMult;		// 5C
	UInt32				unk60;				// 60	Unused; rigged by CopyHitDataHook to store hitLocation
};
static_assert(sizeof(ActorHitData) == 0x64);
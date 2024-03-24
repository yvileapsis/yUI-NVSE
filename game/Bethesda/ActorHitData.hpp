#pragma once

#include "NiPoint3.hpp"

class Actor;
class Projectile;
class Explosion;
class TESObjectWEAP;

struct ActorHitData {
	enum
	{
		kHitData_IsCritical = 0x4,
		kHitData_IsFatal = 0x10,
		kHitData_ExplodePart = 0x40,
		kHitData_DismemberPart = 0x20,
		kHitData_CripplePart = 0x80,
		kHitData_IsSneakAttackCritical = 0x400,
	};

	Actor* source;		// 00
	Actor* target;		// 04
	union								// 08
	{
		Projectile* projectile;
		Explosion* explosion;
	};
	UInt32				unk0C;			// 0C
	SInt32				hitLocation;	// 10
	float				healthDmg;		// 14
	float				wpnBaseDmg;		// 18	Skill and weapon condition modifiers included
	float				fatigueDmg;		// 1C
	float				limbDmg;		// 20
	float				blockDTMod;		// 24
	float				armorDmg;		// 28
	float				flt2C;			// 2C
	TESObjectWEAP*		weapon;		// 30
	float				healthPerc;		// 34
	NiPoint3			impactPos;		// 38
	NiPoint3			impactAngle;	// 44
	UInt32				unk50;			// 50
	UInt32				unk54;			// 54
	UInt32				flags;			// 58
	float				dmgMult;		// 5C
	UInt32				unk60;			// 60	Unused
};
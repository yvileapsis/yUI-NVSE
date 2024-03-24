#pragma once

#include "BGSDestructibleObjectForm.hpp"
#include "BGSPreloadable.hpp"
#include "NiPoint3.hpp"
#include "TESBoundObject.hpp"
#include "TESModel.hpp"

class TESObjectLIGH;
class TESSound;
class TESObjectWEAP;

class BGSProjectile : public TESBoundObject, public TESFullName, public TESModel, public BGSPreloadable, public BGSDestructibleObjectForm {
public:
	BGSProjectile();
	~BGSProjectile();

	enum
	{
		kFlags_Hitscan = 1 << 0,
		kFlags_Explosion = 1 << 1,
		kFlags_AltTrigger = 1 << 2,
		kFlags_MuzzleFlash = 1 << 3,
		//				1 << 4,
		kFlags_CanBeDisabled = 1 << 5,
		kFlags_CanBePicked = 1 << 6,
		kFlags_Supersonic = 1 << 7,
		kFlags_PinsLimbs = 1 << 8,
		kFlags_PassSmallTransparent = 1 << 9,
		kFlags_Detonates = 1 << 10,
		kFlags_Rotation = 1 << 11,
	};

	Bitfield16						usProjFlags;		// 060
	UInt16							usType;				// 062
	float							fGravity;			// 064
	float							fSpeed;				// 068
	float							fRange;				// 06C
	TESObjectLIGH*					pLightProjectile;	// 070
	TESObjectLIGH*					pLightMuzzleFlash;	// 074
	float							fTracerChance;		// 078
	float							fAltProximity;		// 07C
	float							fAltTimer;			// 080
	BGSExplosion*					pExplosionForm;		// 084
	TESSound*						pSoundProjectile;	// 088
	float							fFlashDuration;		// 08C
	float							fFadeDuration;		// 090
	float							fImpactForce;		// 094
	TESSound*						pSoundCountDown;	// 098
	TESSound*						pSoundDisable;		// 09C
	TESObjectWEAP*					pDefaultWeapSrc;	// 0A0
	NiPoint3						kRotation;			// 0A4
	float							fBouncyMult;		// 0B0
	TESModel						kMuzzleFlash;		// 0B4
	UInt8							ucSoundLevel;		// 0CC
};
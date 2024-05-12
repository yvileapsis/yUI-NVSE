#pragma once
#include "NiPoint3.hpp"
#include "TESBoundObject.hpp"
#include "TESModel.hpp"
#include "BGSPreloadable.hpp"
#include "BGSDestructibleObjectForm.hpp"

class TESObjectLIGH;
class TESSound;
class TESObjectWEAP;

// 0xD0
class BGSProjectile :
	public TESBoundObject,				// 000
	public TESFullName,					// 030
	public TESModel,					// 03C
	public BGSPreloadable,				// 054
	public BGSDestructibleObjectForm	// 058
{
public:
	BGSProjectile();
	~BGSProjectile();

	enum EnumFlags : UInt16
	{
		kFlags_Hitscan				= 1 << 0,
		kFlags_Explosion			= 1 << 1,
		kFlags_AltTrigger			= 1 << 2,
		kFlags_MuzzleFlash			= 1 << 3,
		//							= 1 << 4,
		kFlags_CanBeDisabled		= 1 << 5,
		kFlags_CanBePicked			= 1 << 6,
		kFlags_Supersonic			= 1 << 7,
		kFlags_PinsLimbs			= 1 << 8,
		kFlags_PassSmallTransparent	= 1 << 9,
		kFlags_Detonates			= 1 << 10,
		kFlags_Rotation				= 1 << 11,
	};

	enum EnumType : UInt16
	{

	};

	enum EnumSoundLevel : UInt8
	{

	};

	EnumFlags				eFlags;				// 060
	EnumType				eType;				// 062
	Float32					fGravity;			// 064
	Float32					fSpeed;				// 068
	Float32					fRange;				// 06C
	TESObjectLIGH*			pkLightProjectile;	// 070
	TESObjectLIGH*			pkLightMuzzleFlash;	// 074
	Float32					fTracerChance;		// 078
	Float32					fAltProximity;		// 07C
	Float32					fAltTimer;			// 080
	BGSExplosion*			pkExplosion;		// 084
	TESSound*				pkSound;			// 088
	Float32					fFlashDuration;		// 08C
	Float32					fFadeDuration;		// 090
	Float32					fImpactForce;		// 094
	TESSound*				pkSoundCountDown;	// 098
	TESSound*				pkSoundDisable;		// 09C
	TESObjectWEAP*			pkWeaponSource;		// 0A0
	NiPoint3				kRotation;			// 0A4
	
	Float32					fBouncyMult;		// 0B0
	TESModel				kMuzzleFlash;		// 0B4
	EnumSoundLevel			eSoundLevel;		// 0CC

	void SetFlag(EnumFlags pFlag, bool bMod) { eFlags = bMod ? (EnumFlags) (eFlags | pFlag) : (EnumFlags) (eFlags & ~pFlag); }
};
static_assert(sizeof(BGSProjectile) == 0xD0);
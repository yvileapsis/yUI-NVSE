#pragma once
#include "TESForm.hpp"
#include "TESModel.hpp"

class TESTexture;
class TESSound;

// 004 looks to be endian swapped !
struct ColorRGB {
	UInt8	red;	// 000
	UInt8	green;	// 001
	UInt8	blue;	// 002
	UInt8	alpha;	// 003 or unused if no alpha
};

// 024
struct DecalData {
	Float32		fMinWidth;			// 000
	Float32		fMaxWidth;			// 004
	Float32		fMinHeight;			// 008
	Float32		fMaxHeight;			// 00C
	Float32		fDepth;				// 010
	Float32		fShininess;			// 014
	Float32		fParallaxScale;		// 018
	UInt8		ucParallaxPasses;	// 01C
	UInt8		ucFlags;			// 01D	Parallax, Alpha - Blending, Alpha - Testing
	UInt8		unk01E[2];			// 01E
	ColorRGB	kColor;				// 020
};
static_assert(sizeof(DecalData) == 0x024);

// 018
struct ImpactData
{
	Float32		fEffectDuration;		// 000
	UInt32		uiEffectorientation;	// 004	Surface Normal, Projectile vector, Projectile reflection
	Float32		fAngleThreshold;		// 008
	Float32		fPlacementRadius;		// 00C
	UInt32		uiSoundLevel;			// 010	enum
	UInt32		uiFlags;				// 014	No decal data
};
static_assert(sizeof(ImpactData) == 0x18);

class BGSImpactData :
	public TESForm,				// 000
	public TESModel,			// 018
	public ImpactData			// 036
{
public:
	BGSImpactData();
	~BGSImpactData();

	TESTexture* pkTextureSet;	// 048 DNAM Texture Set
	TESSound*	pkSound1;		// 04C
	TESSound*	pkSound2;		// 050
	DecalData	kDecalData;		// 054 DODT [begining of DATA before form version 0x0A]
};
static_assert(sizeof(BGSImpactData) == 0x78);
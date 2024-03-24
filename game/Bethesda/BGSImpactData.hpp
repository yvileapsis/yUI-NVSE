#pragma once

#include "TESForm.hpp"
#include "TESModel.hpp"

class TESTexture;
class TESSound;

struct ColorRGB {
	UInt8	red;	// 000
	UInt8	green;	// 001
	UInt8	blue;	// 002
	UInt8	alpha;	// 003 or unused if no alpha
};	// 004 looks to be endian swapped !

struct DecalData {
	float		minWidth;		// 000
	float		maxWidth;		// 004
	float		minHeight;		// 008
	float		maxHeight;		// 00C
	float		depth;			// 010
	float		shininess;		// 014
	float		parallaxScale;	// 018
	UInt8		parallaxPasses;	// 01C
	UInt8		flags;			// 01D	Parallax, Alpha - Blending, Alpha - Testing
	UInt8		unk01E[2];		// 01E
	ColorRGB	color;			// 020
};	// 024

ASSERT_SIZE(DecalData, 0x024);

class BGSImpactData : public TESForm, public TESModel {
public:
	BGSImpactData();
	virtual ~BGSImpactData();

	struct Data {
		float	effectDuration;		// 000
		UInt32	effectorientation;	// 004	Surface Normal, Projectile vector, Projectile reflection
		float	angleThreshold;		// 008
		float	placementRadius;	// 00C
		UInt32	soundLevel;			// 010	enum
		UInt32	flags;				// 014	No decal data
	};	// 018

	Data		data;			// 030	DATA
	TESTexture* textureSet;	// 048 DNAM Texture Set
	TESSound* sound1;		// 04C
	TESSound* sound2;		// 050
	DecalData	decalData;		// 054 DODT [begining of DATA before form version 0x0A]
};

ASSERT_SIZE(BGSImpactData, 0x078);
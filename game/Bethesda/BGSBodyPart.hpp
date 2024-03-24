#pragma once

#include "BaseFormComponent.hpp"
#include "BSString.hpp"
#include "TESModel.hpp"

class BGSDebris;
class BGSExplosion;
class BGSImpactDataSet;

class BGSBodyPart : public BaseFormComponent {
public:
	BGSBodyPart();
	~BGSBodyPart();

	enum
	{
		kFlags_Severable = 1,
		kFlags_IKData = 2,
		kFlags_BipedData = 4,
		kFlags_Explodable = 8,
		kFlags_IsHead = 16,
		kFlags_Headtracking = 32,
		kFlags_Absolute = 64,
	};

	BSStringT				partNode;				// 04
	BSStringT				VATSTarget;				// 0C
	BSStringT				startNode;				// 14
	BSStringT				partName;				// 1C
	BSStringT				targetBone;				// 24
	TESModel			limbReplacement;		// 2C
	UInt32				unk44[6];				// 44	Another model?
	float				damageMult;				// 5C
	UInt8				flags;					// 60
	UInt8				pad61;					// 61
	UInt8				healthPercent;			// 62
	UInt8				actorValue;				// 63
	UInt8				toHitChance;			// 64
	UInt8				explChance;				// 65
	UInt8				explDebrisCount;		// 66
	UInt8				pad67;					// 67
	BGSDebris* explDebris;			// 68
	BGSExplosion* explExplosion;			// 6C
	float				trackingMaxAngle;		// 70
	float				explDebrisScale;		// 74
	UInt8				sevrDebrisCount;		// 78
	UInt8				pad79[3];				// 79
	BGSDebris* sevrDebris;			// 7C
	BGSExplosion* sevrExplosion;			// 80
	float				sevrDebrisScale;		// 84
	float				goreEffTranslate[3];	// 88
	float				goreEffRotation[3];		// 94
	BGSImpactDataSet* sevrImpactDS;			// A0
	BGSImpactDataSet* explImpactDS;			// A4
	UInt8				sevrDecalCount;			// A8
	UInt8				explDecalCount;			// A9
	UInt8				padAA[2];				// AA
	float				limbRepScale;			// AC

	void SetFlag(UInt32 pFlag, bool bMod)
	{
		flags = bMod ? (flags | pFlag) : (flags & ~pFlag);
	}
};

ASSERT_SIZE(BGSBodyPart, 0xB0);
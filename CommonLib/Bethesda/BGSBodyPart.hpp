#pragma once
#include "BaseFormComponent.hpp"
#include "BSString.hpp"
#include "TESModel.hpp"

class BGSDebris;
class BGSExplosion;
class BGSImpactDataSet;

// 0xB0
class BGSBodyPart : public BaseFormComponent
{
public:
	BGSBodyPart();
	~BGSBodyPart();

	enum EnumFlags : UInt8
	{
		kFlags_Severable		= 1 << 0,
		kFlags_IKData			= 1 << 1,
		kFlags_BipedData		= 1 << 2,
		kFlags_Explodable		= 1 << 3,
		kFlags_IsHead			= 1 << 4,
		kFlags_Headtracking		= 1 << 5,
		kFlags_Absolute			= 1 << 6,
	};

	BSStringT			kPartNode;					// 04
	BSStringT			kVATSTarget;				// 0C
	BSStringT			kStartNode;					// 14
	BSStringT			kPartName;					// 1C
	BSStringT			kTargetBone;				// 24
	TESModel			kLimbReplacement;			// 2C
	UInt32				unk44[6];					// 44	Another model?
	Float32				fDamageMult;				// 5C
	EnumFlags			eFlags;						// 60
	UInt8				pad61;						// 61
	UInt8				ucHealthPercent;			// 62
	UInt8				ucActorValue;				// 63
	UInt8				ucToHitChance;				// 64
	UInt8				ucExplodeableChance;		// 65
	UInt8				ucExplodeableDebrisCount;	// 66
	UInt8				pad67;						// 67
	BGSDebris*			pkExplodeableDebris;		// 68
	BGSExplosion*		pkExplodeableExplosion;		// 6C
	Float32				fTrackingMaxAngle;			// 70
	Float32				fExplodeableDebrisScale;	// 74
	UInt8				ucSeverableDebrisCount;		// 78
	UInt8				pad79[3];					// 79
	BGSDebris*			pkSeverableDebris;			// 7C
	BGSExplosion*		plSeverableExplosion;		// 80
	Float32				fSeverableDebrisScale;		// 84
	Float32				fGoreEffTranslate[3];		// 88
	Float32				fGoreEffRotation[3];		// 94
	BGSImpactDataSet*	pkSeverableImpactDataSet;	// A0
	BGSImpactDataSet*	plExplodeableImpactDS;		// A4
	UInt8				ucSeverableDecalCount;		// A8
	UInt8				ucExplodeableDecalCount;	// A9
	UInt8				padAA[2];					// AA
	Float32				fLimbRepScale;				// AC

	void SetFlag(EnumFlags apFlag, bool abMod) { eFlags = (EnumFlags)(abMod ? (eFlags | apFlag) : (eFlags & ~apFlag)); }
};
static_assert(sizeof(BGSBodyPart) == 0xB0);
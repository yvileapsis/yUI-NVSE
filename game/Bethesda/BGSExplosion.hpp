#pragma once
#include "TESBoundObject.hpp"
#include "TESModel.hpp"
#include "TESEnchantableForm.hpp"
#include "BGSPreloadable.hpp"
#include "TESImageSpaceModifiableForm.hpp"

class TESObjectLIGH;
class TESSound;
class BGSImpactDataSet;

// 0xA4
class BGSExplosion :
	public TESBoundObject,				// 000
	public TESFullName,					// 030
	public TESModel,					// 03C
	public TESEnchantableForm,			// 054
	public BGSPreloadable,				// 064
	public TESImageSpaceModifiableForm	// 068
{
public:
	BGSExplosion();
	~BGSExplosion();

	enum EnumFlags
	{
		kFlags_Unknown						=	1 << 0,
		kFlags_AlwaysUseWorldOrientation	=	1 << 1,
		kFlags_KnockDownAlways				=	1 << 2,
		kFlags_KnockDownByFormula			=	1 << 3,
		kFlags_IgnoreLOSCheck				=	1 << 4,
		kFlags_PushSourceRefOnly			=	1 << 5,
		kFlags_IgnoreImageSpaceSwap			=	1 << 6,
	};

	TESForm*					pkPlacedObject;			// 070
	Float32						fForce;					// 074
	Float32						fDamage;				// 078
	Float32						fRadius;				// 07C
	TESObjectLIGH*				pkLight;				// 080
	TESSound*					pkSound1;				// 084
	EnumFlags					eFlags;					// 088
	Float32						fISradius;				// 08C
	BGSImpactDataSet*			pkImpactDataSet;		// 090
	TESSound*					pkSound2;				// 094
	Float32						fRadiationlevel;		// 098
	Float32						fDissipationTime;		// 09C
	Float32						fRadiationRadius;		// 0A0

	void SetFlag(EnumFlags aeFlag, bool abVal) { eFlags = (EnumFlags)(abVal ? (eFlags | aeFlag) : (eFlags & ~aeFlag)); }
};
static_assert(sizeof(BGSExplosion) == 0xA4);
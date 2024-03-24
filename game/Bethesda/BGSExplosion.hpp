#pragma once

#include "TESBoundObject.hpp"
#include "TESModel.hpp"
#include "TESEnchantableForm.hpp"
#include "BGSPreloadable.hpp"
#include "TESImageSpaceModifiableForm.hpp"

class TESObjectLIGH;
class TESSound;
class BGSImpactDataSet;

class BGSExplosion : public TESBoundObject {
public:
	BGSExplosion();
	~BGSExplosion();

	enum
	{
		kFlags_Unknown = 1,
		kFlags_AlwaysUseWorldOrientation = 2,
		kFlags_KnockDownAlways = 4,
		kFlags_KnockDownByFormula = 8,
		kFlags_IgnoreLOSCheck = 16,
		kFlags_PushSourceRefOnly = 32,
		kFlags_IgnoreImageSpaceSwap = 64,
	};

	TESFullName					fullName;			// 030
	TESModel					model;				// 03C
	TESEnchantableForm			enchantable;		// 054
	BGSPreloadable				preloadable;		// 064
	TESImageSpaceModifiableForm	imageSpaceModForm;	// 068

	TESForm* placedObj;			// 070
	float						force;				// 074
	float						damage;				// 078
	float						radius;				// 07C
	TESObjectLIGH* light;				// 080
	TESSound* sound1;			// 084
	UInt32						explFlags;			// 088
	float						ISradius;			// 08C
	BGSImpactDataSet* impactDataSet;		// 090
	TESSound* sound2;			// 094
	float						RADlevel;			// 098
	float						dissipationTime;	// 09C
	float						RADradius;			// 0A0

	void SetFlag(UInt32 flag, bool val)
	{
		explFlags = val ? (explFlags | flag) : (explFlags & ~flag);
	}
};

ASSERT_SIZE(BGSExplosion, 0xA4);
#pragma once

#include "TESBoundObject.hpp"
#include "TESFullName.hpp"
#include "TESModelTextureSwap.hpp"
#include "TESIcon.hpp"
#include "BGSMessageIcon.hpp"
#include "TESValueForm.hpp"
#include "BGSClipRoundsForm.hpp"
#include "BGSDestructibleObjectForm.hpp"
#include "BGSPickupPutdownSounds.hpp"
#include "TESScriptableForm.hpp"

class BGSProjectile;
class TESObjectMISC;
class TESAmmoEffect;

class TESAmmo : public TESBoundObject {
public:
	TESAmmo();
	~TESAmmo();

	enum eAmmoFlags
	{
		kFlags_IgnoreWeapResistance = 1,
		kFlags_NonPlayable = 2,
	};

	// bases
	TESFullName					fullName;				// 030
	TESModelTextureSwap			model;					// 03C
	TESIcon						icon;					// 05C
	BGSMessageIcon				messageIcon;			// 068
	TESValueForm				value;					// 078
	BGSClipRoundsForm			clipRounds;				// 080
	BGSDestructibleObjectForm	destructible;			// 088
	BGSPickupPutdownSounds		pickupPutdownsounds;	// 090
	TESScriptableForm			scriptable;				// 09C

	float						speed;					// 0A8	8 bytes makes it DATA
	UInt32						flags;					// 0AC	only 8 bits most likely. 0 is Ignores Normal Weapon Resistance', 1 is Non-Playable
	UInt32						projPerShot;			// 0B0
	BGSProjectile* projectile;			// 0B4
	float						weight;					// 0B8
	TESObjectMISC* casing;				// 0BC
	float						ammoPercentConsumed;	// 0C0
	BSStringT						shortName;				// 0C4
	BSStringT						abbreviation;			// 0CC
	BSSimpleList<TESAmmoEffect*>		effecBSSimpleList;				// 0D4

	bool IsNonPlayable() { return (flags & kFlags_NonPlayable) == kFlags_NonPlayable; }
	bool IsPlayable() { return !IsNonPlayable(); }
	SInt32 GetEffectIndex(TESAmmoEffect* effect);
};

ASSERT_SIZE(TESAmmo, 0xDC);
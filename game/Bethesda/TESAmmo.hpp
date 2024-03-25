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

// 0xDC
class TESAmmo :
	public TESBoundObject,				// 000
	public TESFullName,					// 030
	public TESModelTextureSwap,			// 03C
	public TESIcon,						// 05C
	public BGSMessageIcon,				// 068
	public TESValueForm,				// 078
	public BGSClipRoundsForm,			// 080
	public BGSDestructibleObjectForm,	// 088
	public BGSPickupPutdownSounds,		// 090
	public TESScriptableForm			// 09C
{
public:
	TESAmmo();
	~TESAmmo();

	enum EnumAmmoFlags : UInt32
	{
		kFlags_IgnoreWeapResistance = 1,
		kFlags_NonPlayable = 2,
	};

	Float32							speed;					// 0A8	8 bytes makes it DATA
	EnumAmmoFlags					eFlags;					// 0AC	only 8 bits most likely. 0 is Ignores Normal Weapon Resistance', 1 is Non-Playable
	UInt32							uiProjPerShot;			// 0B0
	BGSProjectile*					pkProjectile;			// 0B4
	Float32							fWeight;				// 0B8
	TESObjectMISC*					pkCasing;				// 0BC
	Float32							fAmmoPercentConsumed;	// 0C0
	BSStringT<char>					kShortName;				// 0C4
	BSStringT<char>					kAbbreviation;			// 0CC
	BSSimpleList<TESAmmoEffect*>	kEffectList;			// 0D4

	bool IsNonPlayable() { return (eFlags & kFlags_NonPlayable) == kFlags_NonPlayable; }
	bool IsPlayable() { return !IsNonPlayable(); }
	SInt32 GetEffectIndex(TESAmmoEffect* effect);
};
static_assert(sizeof(TESAmmo) == 0xDC);
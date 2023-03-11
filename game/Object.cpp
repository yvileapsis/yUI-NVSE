#include <Object.h>

#include "GameRTTI.h"
#include "Reference.h"

static constexpr UInt8 kHandGripTable[] =
{
	TESObjectWEAP::eHandGrip_Default,
	TESObjectWEAP::eHandGrip_1,
	TESObjectWEAP::eHandGrip_2,
	TESObjectWEAP::eHandGrip_3,
	TESObjectWEAP::eHandGrip_4,
	TESObjectWEAP::eHandGrip_5,
	TESObjectWEAP::eHandGrip_6,
};

bool TESObjectWEAP::IsMeleeWeapon() const
{
	return eWeaponType >= 0 && eWeaponType <= 2;
}

float TESObjectWEAP::GetModBonuses(UInt32 effectID)
{
	float result = 0;
	for (UInt32 idx = 0; idx < 3; idx++) if (effectMods[idx] == effectID) result += value1Mod[idx];
	return result;
}

UInt8 TESObjectWEAP::HandGrip() const
{
	for (UInt32 i = 0; i < std::size(kHandGripTable); i++) if (handGrip == kHandGripTable[i]) return i;
	return 0;
}

void TESObjectWEAP::SetHandGrip(UInt8 _handGrip)
{
	if (_handGrip < std::size(kHandGripTable)) handGrip = kHandGripTable[_handGrip];
}

UInt8 TESObjectWEAP::AttackAnimation() const
{
	switch (attackAnim) {
	case eAttackAnim_Default:		return 0;
	case eAttackAnim_Attack3:		return 1;
	case eAttackAnim_Attack4:		return 2;
	case eAttackAnim_Attack5:		return 3;
	case eAttackAnim_Attack6:		return 4;
	case eAttackAnim_Attack7:		return 5;
	case eAttackAnim_Attack8:		return 6;
	case eAttackAnim_AttackLeft:	return 7;
	case eAttackAnim_AttackLoop:	return 8;
	case eAttackAnim_AttackRight:	return 9;
	case eAttackAnim_AttackSpin:	return 10;
	case eAttackAnim_AttackSpin2:	return 11;
	case eAttackAnim_AttackThrow:	return 12;
	case eAttackAnim_AttackThrow2:	return 13;
	case eAttackAnim_AttackThrow3:	return 14;
	case eAttackAnim_AttackThrow4:	return 15;
	case eAttackAnim_AttackThrow5:	return 16;
	case eAttackAnim_PlaceMine:		return 17;
	case eAttackAnim_PlaceMine2:	return 18;
	default:						return -1;
	}
}

void TESObjectWEAP::SetAttackAnimation(UInt8 _attackAnim)
{
	switch (_attackAnim) {
	case 0: {
		attackAnim = eAttackAnim_Default;
		break;
	}
	case 1: {
		attackAnim = eAttackAnim_Attack3;
		break;
	}
	case 2: {
		attackAnim = eAttackAnim_Attack4;
		break;
	}
	case 3: {
		attackAnim = eAttackAnim_Attack5;
		break;
	}
	case 4: {
		attackAnim = eAttackAnim_Attack6;
		break;
	}
	case 5: {
		attackAnim = eAttackAnim_Attack7;
		break;
	}
	case 6: {
		attackAnim = eAttackAnim_Attack8;
		break;
	}

	case 7: {
		attackAnim = eAttackAnim_AttackLeft;
		break;
	}
	case 8: {
		attackAnim = eAttackAnim_AttackLoop;
		break;
	}
	case 9: {
		attackAnim = eAttackAnim_AttackRight;
		break;
	}
	case 10: {
		attackAnim = eAttackAnim_AttackSpin;
		break;
	}
	case 11: {
		attackAnim = eAttackAnim_AttackSpin2;
		break;
	}
	case 12: {
		attackAnim = eAttackAnim_AttackThrow;
		break;
	}
	case 13: {
		attackAnim = eAttackAnim_AttackThrow2;
		break;
	}
	case 14: {
		attackAnim = eAttackAnim_AttackThrow3;
		break;
	}
	case 15: {
		attackAnim = eAttackAnim_AttackThrow4;
		break;
	}
	case 16: {
		attackAnim = eAttackAnim_AttackThrow5;
		break;
	}
	case 17: {
		attackAnim = eAttackAnim_PlaceMine;
		break;
	}
	case 18: {
		attackAnim = eAttackAnim_PlaceMine2;
		break;
	}
	default:
		break;
	}
}

TESObjectIMOD* TESObjectWEAP::GetItemMod(UInt8 which)
{
	TESObjectIMOD* pMod = nullptr;
	switch (which) {
	case 1: pMod = itemMod1; break;
	case 2: pMod = itemMod2; break;
	case 3: pMod = itemMod3; break;
	default: break;
	}
	return pMod;
}

Float32 TESObjectWEAP::GetEffectModValue(kWeaponModEffects value, UInt8 second)
{
	if (value == GetItemModEffect(1)) return second ? GetItemModValue2(1) : GetItemModValue1(1);
	if (value == GetItemModEffect(2)) return second ? GetItemModValue2(2) : GetItemModValue1(2);
	if (value == GetItemModEffect(3)) return second ? GetItemModValue2(3) : GetItemModValue1(3);
	return -1;
}

bool TESObjectWEAP::IsMeleeWeapon()
{
	return eWeaponType == kWeapType_HandToHandMelee || eWeaponType == kWeapType_OneHandMelee || eWeaponType == kWeapType_TwoHandMelee;
}

bool TESObjectWEAP::IsRangedWeapon()
{
	return (this->projectile);
}


float TESObjectWEAP::GetWeaponValue(UInt32 whichVal)
{
	if (!this) return false;
	switch (whichVal) {
	case eWeap_Type:				return eWeaponType;
	case eWeap_MinSpread:			return minSpread;
	case eWeap_Spread:				return spread;
	case eWeap_Proj:				return projectile ? projectile->refID : false;
	case eWeap_SightFOV:			return sightFOV;
	case eWeap_MinRange:			return minRange;
	case eWeap_Range:				return maxRange;
	case eWeap_AmmoUse:				return ammoUse;
	case eWeap_APCost:				return AP;
	case eWeap_CritDam:				return criticalDamage;
	case eWeap_CritChance:			return criticalPercent;
	case eWeap_CritEffect:			return criticalEffect ? criticalEffect->refID : false;
	case eWeap_FireRate:			return fireRate;
	case eWeap_AnimAttackMult:		return animAttackMult;
	case eWeap_RumbleLeft:			return rumbleLeftMotor;
	case eWeap_RumbleRight:			return rumbleRightMotor;
	case eWeap_RumbleDuration:		return rumbleDuration;
	case eWeap_RumbleWaveLength:	return rumbleWavelength;
	case eWeap_AnimShotsPerSec:		return animShotsPerSec;
	case eWeap_AnimReloadTime:		return animReloadTime;
	case eWeap_AnimJamTime:			return animJamTime;
	case eWeap_Skill:				return weaponSkill;
	case eWeap_ResistType:			return resistType;
	case eWeap_FireDelayMin:		return semiAutoFireDelayMin;
	case eWeap_FireDelayMax:		return semiAutoFireDelayMax;
	case eWeap_AnimMult:			return animMult;
	case eWeap_Reach:				return reach;
	case eWeap_IsAutomatic:			return IsAutomatic();
	case eWeap_HandGrip:			return HandGrip();
	case eWeap_ReloadAnim:			return reloadAnim;
	case eWeap_VATSChance:			return baseVATSChance;
	case eWeap_AttackAnim:			return AttackAnimation();
	case eWeap_NumProj:				return numProjectiles;
	case eWeap_AimArc:				return aimArc;
	case eWeap_LimbDamageMult:		return limbDamageMult;
	case eWeap_SightUsage:			return sightUsage;
	case eWeap_ReqStr:				return strRequired;
	case eWeap_ReqSkill:			return skillRequirement;
	case eWeap_LongBursts:			return weaponFlags2 & (eFlag_LongBurst);
	case eWeap_Flags1:				return weaponFlags1;
	case eWeap_Flags2:				return weaponFlags2;
	case eWeap_HasScope:			return HasScope();
	case eWeap_IgnoresDTDR:			return IgnoresDTDR();
	case eWeap_SoundLevel:			return soundLevel;
	case eWeap_ClipSize:
	{
		if (const auto pClipRounds = DYNAMIC_CAST(this, TESForm, BGSClipRoundsForm); pClipRounds) return pClipRounds->clipRounds;
		break;
	}
	default: throw std::out_of_range("unknown weapon value");
	}
	return false;
}

UInt32 TESObjectARMO::GetArmorValue(UInt32 whichVal)
{
	if (!this) return false;
	switch (whichVal) {
	case 1:	return bipedModel.bipedFlags & eBipedFlag_HeavyArmor ? 3 : bipedModel.bipedFlags & eBipedFlag_MediumArmor ? 2 : 1;
	case 2:	return bipedModel.bipedFlags & eBipedFlag_PowerArmor ? 1 : 0;
	case 3:	return bipedModel.bipedFlags & eBipedFlag_HasBackPack ? 1 : 0;
	case 4:	return armorRating;
	case 5:	return static_cast<UInt32>(damageThreshold);
	case 6: return bipedModel.partMask;
	default: throw std::out_of_range("unknown armor value");
	}
	return false;
}

UInt32 AlchemyItem::HasBaseEffectRestoresAV(const SInt32 avCode)
{
	for (const auto effect : magicItem.list.list)
		if (effect->GetSkillCode() == avCode && (effect->setting && !(effect->setting->effectFlags & EffectSetting::kDetrimental)))
			return effect->magnitude;
	return 0;
}

UInt32 AlchemyItem::HasBaseEffectDamagesAV(const SInt32 avCode)
{
	for (const auto effect : magicItem.list.list)
		if (effect->GetSkillCode() == avCode && (effect->setting && effect->setting->effectFlags & EffectSetting::kDetrimental))
			return effect->magnitude;
	return 0;
}

bool AlchemyItem::IsAddictive()
{
	return withdrawalEffect && withdrawalEffect->refID;
}

bool AlchemyItem::IsFood()
{
	return alchFlags & 2;
}

bool AlchemyItem::IsMedicine()
{
	return alchFlags & 4;
}

bool AlchemyItem::IsPoison()
{
	EffectSetting* effSetting = nullptr;
	for (const auto effItem : magicItem.list.list)
	{
		effSetting = effItem->setting;
		if (effSetting && !(effSetting->effectFlags & 4)) return false;
	}

	return effSetting != nullptr;
}

bool AlchemyItem::IsFoodAlt()
{
	return HasBaseEffectRestoresAV(kAVCode_Hunger) && HasBaseEffectRestoresAV(kAVCode_Hunger) > HasBaseEffectRestoresAV(kAVCode_Dehydration) ? true : false;
}

bool AlchemyItem::IsWaterAlt()
{
	return HasBaseEffectRestoresAV(kAVCode_Dehydration) && HasBaseEffectRestoresAV(kAVCode_Hunger) == 0 ? true : false;
}

bool HasBaseEffectChangesAV(TESForm* form, const int avCode)
{
	const auto armor = DYNAMIC_CAST(form, TESForm, TESObjectARMO);
	if (!armor) return false;
	const auto enchantment = armor->enchantable.enchantItem;
	if (!enchantment) return false;
	for (const auto effect : enchantment->magicItem.list.list)
		if (effect->GetSkillCode() == avCode && (effect->setting && effect->setting->effectFlags & EffectSetting::kRecover))
			return true;
	return false;
}


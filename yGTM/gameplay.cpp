#include <gameplay.h>
#include <GameObjects.h>
#include <GameRTTI.h>

#include "GameProcess.h"
#include "GameSettings.h"

Float64 __cdecl AdjustPushForceAlt(Actor* target, ActorHitData* hitdata, ActorValueOwner* owner, SInt32 force)
{
	Float64 scale = 1.0;
	if (hitdata && hitdata->explosion && hitdata->explosion->baseForm)
	{
		const auto explosion = DYNAMIC_CAST(hitdata->explosion->baseForm, TESForm, BGSExplosion);
		scale = CdeclCall<Float64>(0x647920, hitdata->explosion->radius, hitdata->explosion->GetDistance(target));
		scale *= explosion->force / 100.0;
	}
	return scale * CdeclCall<Float64>(0x646580, owner, force);
}

Float32 __fastcall SetSpreadForActor(Actor* actor) {
	const auto conditionTier = ThisStdCall<SInt32>(0x92B9E0, actor->baseProcess, actor);
    if (conditionTier > 9) return 0.5;
    const auto spreadArray = reinterpret_cast<Setting**>(0x119B2BC);
    const Setting* spreadSetting = spreadArray[conditionTier + 10];
    const float result = spreadSetting->data.f;
	actor->flt178 = result;
    return result;
}

void __fastcall StoreAmmoOnProjectile(Projectile* projectile)
{
//	projectile->ammo = projectile->weapon->GetEquippedAmmo(static_cast<Actor*>(projectile->source));
	const auto highProcess = reinterpret_cast<HighProcess*>(reinterpret_cast<Actor*>(projectile->source)->baseProcess);
	if (!highProcess || !highProcess->ammoInfo) return;
	projectile->ammo = highProcess->ammoInfo->ammo;
}

void __fastcall StoreAmmoOnExplosion(Projectile* projectile, Explosion* explosion)
{
	explosion->weapon = projectile->weapon;
	if (!explosion->ammo && explosion->CanStoreAmmo()) explosion->ammo = projectile->ammo;
}

void __fastcall GetAmmoFromHitData(ActorHitData* hitData, Actor* target)
{
	if (!hitData || !hitData->projectile) return;
	if (const auto script = hitData->GetAmmoScript()) ThisCall<char>(0x5AC340, script, target, 0);
}

bool __fastcall GetCanNotStoreAmmo(Explosion* explosion)
{
	if (!explosion || !explosion->IsExplosion()) return false;
	if (!explosion->ammo) return false;
	if (!explosion->CanStoreAmmo()) return false;
	return true;
}

TESObjectWEAP* __fastcall MeltdownPassHitDataWeapon(ActorHitData* hitData)
{
	return hitData->GetWeapon();
}

void __fastcall MeltdownSetWeaponAndAmmo(ActorHitData* hitData, Explosion* explosion)
{
	if (!hitData || !explosion || !hitData->projectile) return;
	explosion->weapon = hitData->GetWeapon();
	explosion->ammo = hitData->GetAmmo();
}

std::unordered_map<MiddleHighProcess*, TESAmmo*> g_ammo;
std::unordered_map<MiddleHighProcess*, TESObjectWEAP*> g_weapon;

void PreCalculateHitDamage(ActorHitData* hitData)
{
	if (!hitData || !hitData->source || !hitData->source->baseProcess) return;
	auto process = reinterpret_cast<MiddleHighProcess*>(hitData->source->baseProcess);

	if (!process->GetWeaponInfo() || !process->ammoInfo || !process->ammoInfo->ammo) return;
	g_ammo.emplace(process, process->ammoInfo->ammo);
	if (const auto ammo = hitData->GetAmmo()) process->ammoInfo->ammo = ammo;

	if (!process->GetAmmoInfo() || !process->weaponInfo || !process->weaponInfo->weapon) return;
	g_weapon.emplace(process, process->weaponInfo->weapon);
	if (const auto weapon = hitData->GetWeapon()) process->weaponInfo->weapon = weapon;
}

void PostCalculateHitDamage(ActorHitData* hitData)
{
	if (!hitData || !hitData->source || !hitData->source->baseProcess) return;
	const auto process = reinterpret_cast<MiddleHighProcess*>(hitData->source->baseProcess);

	if (!process->GetWeaponInfo() || !process->ammoInfo || !process->ammoInfo->ammo) return;
	if (g_weapon.contains(process))
	{
		process->weaponInfo->weapon = g_weapon[process];
		g_weapon.erase(process);
	}

	if (!process->GetAmmoInfo() || !process->weaponInfo || !process->weaponInfo->weapon) return;
	if (g_ammo.contains(process))
	{
		process->ammoInfo->ammo = g_ammo[process];
		g_ammo.erase(process);
	}

}

void __fastcall PreCalculateHitDamage1(ActorHitData* hitData)
{
	PreCalculateHitDamage(hitData);
}

void __fastcall PostCalculateHitDamageHook1(ActorHitData* hitData, void* dummyedx, Projectile* projectile)
{
	ThisCall<void>(0x9B6620, hitData, projectile);
	PostCalculateHitDamage(hitData);
}

void __fastcall PostCalculateHitDamageHook2(ActorHitData* hitData)
{
	ThisCall<void>(0x9B73D0, hitData);
	PostCalculateHitDamage(hitData);
}

void __fastcall PostCalculateHitDamageHook3(ActorHitData* hitData)
{
	ThisCall<void>(0x9B7060, hitData);
	PostCalculateHitDamage(hitData);
}

UInt8 __fastcall TESObjectWEAPGetNumProjectilesHook(TESObjectWEAP* weapon, void* dummyEdx, char hasWeaponMod, char dontCheckAmmo, TESForm* form)
{
	auto count = weapon->numProjectiles;
	if (hasWeaponMod) count += ThisCall<Float64>(0x4BCF60, weapon, TESObjectWEAP::kWeaponModEffect_SplitBeam, 0);
	if (form && !dontCheckAmmo) {
		const auto ammo = form->typeID == kFormType_TESAmmo
			? DYNAMIC_CAST(form, TESForm, TESAmmo)
			: weapon->GetEquippedAmmo(DYNAMIC_CAST(form, TESForm, Actor));
		if (ammo && ammo->projPerShot > 1) count *= ammo->projPerShot;
	}
	return count;
}


/*
char __fastcall Test1(MagicCaster* caster, void* dummyedx, TESObjectWEAP* a3)
{
	return ThisCall<char>(0x815870, caster, a3);
}

ActiveEffect* __fastcall Test2(MagicCaster* wah, void* dummyedx, MagicItem* magicitem, EffectItem* effectitem, TESForm* weapon)
{
	return wah->CreateActiveEffect(magicitem, effectitem, weapon);
}
*/


TESObjectWEAP* __fastcall EffectGetWeapon(ContChangesEntry* entry, Projectile* projectile)
{
	if (projectile && projectile->weapon) return projectile->weapon;
	return reinterpret_cast<TESObjectWEAP*>(entry->form);
}

EnchantmentItem* __fastcall EffectGetEnchantment(ContChangesEntry* entry, Projectile* projectile)
{
	TESObjectWEAP* weapon = nullptr;
	if (projectile) weapon = projectile->weapon;
	if (!weapon && entry) weapon = reinterpret_cast<TESObjectWEAP*>(entry->form);
	const auto enchantable = DYNAMIC_CAST(weapon, TESObjectWEAP, TESEnchantableForm);
	auto enchantItem = enchantable->enchantItem;
	if (!enchantable && projectile && projectile->baseForm)
	{
		const auto explosion = reinterpret_cast<BGSProjectile*>(projectile->baseForm)->explosionForm;
		if (const auto enchantable2 = DYNAMIC_CAST(explosion, BGSExplosion, TESEnchantableForm)) enchantItem = enchantable2->enchantItem;
	}
	return enchantItem;
}

ExtraDataList* __fastcall EffectGetPoison(ContChangesEntry* entry, Projectile* projectile)
{
	if (!entry) return nullptr;
	return entry->GetCustomExtra(kExtraData_Poison);
}
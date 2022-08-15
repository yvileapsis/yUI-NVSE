#pragma once
#include "GameObjects.h"
#include "GameExtraData.h"

Float64 __cdecl AdjustPushForceAlt(Actor* target, ActorHitData* hitdata, ActorValueOwner* owner, SInt32 force);

template <UInt32 retn> __declspec(naked) void HitKnockbackHook()
{
	static const auto retnAddr = retn;
	static const auto AdjustPushForce = reinterpret_cast<UInt32>(AdjustPushForceAlt);
	_asm
	{
		mov		eax, [ebx + 0x8] // hitData
		push	eax
		mov     eax, [ebp - 0x18] // target
		push	eax
		call	AdjustPushForce
		add		esp, 8
		jmp		retnAddr
	}
}

Float32 __fastcall GetMinSpread(Actor* actor);
template <UInt32 retn> __declspec(naked) void RestoreMinSpreadHook() {
	static const auto retnAddr = retn;
	__asm {
		call GetMinSpread
		fstp[ebp - 0x10]
		jmp retnAddr
	}
}

Float64 __fastcall GetSpreadCondition(Actor* actor);
template <UInt32 retn> __declspec(naked) void RestoreSpreadConditionHook() {
	static const auto retnAddr = retn;
	__asm {
		mov ecx, [ebp - 0x20]
		call GetSpreadCondition
		jmp retnAddr
	}
}

bool AlterSpread();
Float64 __fastcall AlterSpreadCalculation(Actor* actor, TESObjectWEAP* weapon, Float32 condition, char isAiming, char isSneaking, char isWalking, char isRunning);
template <UInt32 retn1, UInt32 retn2> __declspec(naked) void AlterSpreadHook() {
	static const auto retnAddr1 = retn1;
	static const auto retnAddr2 = retn2;
	__asm {
		call AlterSpread
		test al, al
		jz no

		movzx eax, [ebp + 0x20] // running
		push eax
		movzx eax, [ebp + 0x1C] // walking
		push eax
		movzx ecx, [ebp + 0x18] // sneaking
		push ecx
		movzx edx, [ebp + 0x14] // aiming
		push edx
		mov eax, [ebp + 0x10] // condition
		push eax
		mov edx, [ebp + 0x0C] // weapon
		mov ecx, [ebp + 0x08] // actor

		call AlterSpreadCalculation
		fstp [ebp - 0x24]

		jmp retnAddr2

	no:
		movzx ecx, [ebp + 0x18]
		test ecx, ecx
		jmp retnAddr1
	}
}

void __fastcall StoreAmmoOnProjectile(Projectile* projectile);

template <UInt32 retn> __declspec(naked) void StoreAmmoOnProjectileHook()
{
	static const auto retnAddr = retn;
	static const auto func = reinterpret_cast<UInt32>(StoreAmmoOnProjectile);
	_asm
	{
		mov		ecx, [ebp - 0x28]	// this
		call	func
		jmp		retnAddr
	}
}

void __fastcall StoreAmmoOnExplosion(Projectile* projectile, Explosion* explosion);

template <UInt32 retn> __declspec(naked) void StoreAmmoOnExplosionHook()
{
	static const auto retnAddr = retn;
	static const auto func = reinterpret_cast<UInt32>(StoreAmmoOnExplosion);
	_asm
	{
		mov     edx, [ebp - 0x3C]			// explosion
		call	func
		jmp		retnAddr
	}
}

void __fastcall GetAmmoFromHitData(ActorHitData* hitData, Actor* target);

template <UInt32 retn> __declspec(naked) void GetAmmoFromHitDataHook()
{
	static const auto retnAddr = retn;
	static const auto func = reinterpret_cast<UInt32>(GetAmmoFromHitData);
	_asm
	{
		mov		ecx, [ebx + 0x8]			// this
		mov     edx, [ebp - 0x18]			// target
		call	func
		jmp		retnAddr
	}
}

bool __fastcall GetCanNotStoreAmmo(Explosion* explosion);

template <UInt32 retn> __declspec(naked) void FixExplosionAmmoHook1()
{
	static const auto retnAddr = retn;
	static const auto check = reinterpret_cast<UInt32>(GetCanNotStoreAmmo);
	_asm
	{
		mov     ecx, edx
		call	check
		test	al, al
		jmp		retnAddr
	}
}

template <UInt32 retn> __declspec(naked) void FixExplosionAmmoHook2()
{
	static const auto retnAddr = retn;
	static const auto check = reinterpret_cast<UInt32>(GetCanNotStoreAmmo);
	_asm
	{
		call	check
		mov		dl, al
		jmp		retnAddr
	}
}

template <UInt32 retn> __declspec(naked) void FixExplosionAmmoHook3()
{
	static const auto retnAddr = retn;
	static const auto check = reinterpret_cast<UInt32>(GetCanNotStoreAmmo);
	_asm
	{
		mov     ecx, eax
		call	check
		test	al, al
		jmp		retnAddr
	}
}

TESObjectWEAP* __fastcall MeltdownPassHitDataWeapon(ActorHitData* hitData);

template <UInt32 retn> __declspec(naked) void MeltdownPassHitDataWeaponHook()
{
	static const auto retnAddr = retn;
	static const auto func = reinterpret_cast<UInt32>(MeltdownPassHitDataWeapon);
	_asm
	{
		mov     ecx, [ebx + 8]
		call	func
		mov     dword ptr[ebp - 0x2CC], eax
		jmp		retnAddr
	}
}

void __fastcall MeltdownSetWeaponAndAmmo(ActorHitData* hitData, Explosion* explosion);

template <UInt32 retn> __declspec(naked) void MeltdownSetWeaponAndAmmoHook()
{
	static const auto retnAddr = retn;
	static const auto func = reinterpret_cast<UInt32>(MeltdownSetWeaponAndAmmo);
	_asm
	{
		mov     edx, [ebp - 2E4h]
		call	func
		jmp		retnAddr
	}
}

void __fastcall PreCalculateHitDamage1(ActorHitData* hitData);

template <UInt32 retn> __declspec(naked) void PreCalculateHitDamageHook()
{
	static const auto retnAddr = retn;
	static const auto func = reinterpret_cast<UInt32>(PreCalculateHitDamage1);
	static constexpr auto CalculateHitDamage = 0x9B5A30;
	_asm
	{
		push	ecx
		call	func
		pop		ecx
		push	retnAddr
		jmp		CalculateHitDamage
	}
}

void __fastcall PostCalculateHitDamageHook1(ActorHitData* hitData, void* dummyedx, Projectile* projectile);
void __fastcall PostCalculateHitDamageHook2(ActorHitData* hitData);
void __fastcall PostCalculateHitDamageHook3(ActorHitData* hitData);
UInt8 __fastcall TESObjectWEAPGetNumProjectilesHook(TESObjectWEAP* weapon, void* dummyEdx, char hasWeaponMod, char dontCheckAmmo, TESForm* form);

TESObjectWEAP* __fastcall EffectGetWeapon(ContChangesEntry* entry, Projectile* projectile);
EnchantmentItem* __fastcall EffectGetEnchantment(ContChangesEntry* entry, Projectile* projectile);
AlchemyItem* __fastcall EffectGetPoison(ContChangesEntry* entry, Projectile* projectile);

template <UInt32 retn> __declspec(naked) void WeaponEffectHook1()
{
	static const auto retnAddr = retn;
	static const auto func1 = reinterpret_cast<UInt32>(EffectGetWeapon);
	static const auto func2 = reinterpret_cast<UInt32>(EffectGetEnchantment);
	static const auto func3 = reinterpret_cast<UInt32>(EffectGetPoison);
	__asm {
		mov ecx, [ebp + 0x8]
		mov edx, [ebp + 0x10]
		call func1
		mov[ebp - 0x18], eax

		mov ecx, [ebp + 0x8]
		mov edx, [ebp + 0x10]
		call func2
		mov[ebp - 0x14], eax

		mov ecx, [ebp + 0x8]
		mov edx, [ebp + 0x10]
		call func3
		mov[ebp - 0x10], eax

		jmp retnAddr
	}
}

template <UInt32 retn> __declspec(naked) void WeaponEffectHook2()
{
	static const auto retnAddr = retn;
	static const auto func1 = reinterpret_cast<UInt32>(EffectGetWeapon);
	__asm {
		mov ecx, [ebp + 0x8]
		mov edx, [ebp + 0x10]
		call func1
		mov[ebp - 0x38], eax

		jmp retnAddr
	}
}

char __fastcall MergeScriptEvent(const ActorHitData*);

template <UInt32 retn> __declspec(naked) void MergeScriptEventHook() {
	static const auto retnAddr = retn;
	static const auto SpreadFunc = MergeScriptEvent;
	__asm {
		mov ecx, [ebx + 0x8]
		call SpreadFunc
		jmp retnAddr
	}
}

namespace ArmedUnarmed
{
	bool __fastcall ShouldNotShowAmmo(TESObjectWEAP* weapon);

	bool __fastcall QueueAttackFireCheck(TESObjectWEAP* weapon, Actor* actor);

	template <UInt32 retn> __declspec(naked) void QueueAttackFireHook() {
		static const auto retnAddr = retn;
		static const auto check = QueueAttackFireCheck;
		__asm {
			mov edx, [ebp - 0x10C]
			call check
			jmp retnAddr
		}
	}

	void __fastcall QueuePowerAttackFireCheck(TESObjectWEAP* weapon, Actor* actor);

	template <UInt32 retn> __declspec(naked) void QueuePowerAttackFireHook() {
		static const auto retnAddr = retn;
		static const auto check = QueuePowerAttackFireCheck;
		__asm {
			mov ecx, [ebp - 0x38]
			mov edx, [ebp - 0x10C]
			call check
			mov ecx, [ebp - 0x10C]
			jmp retnAddr
		}
	}

	bool __fastcall ExecuteAttackHook(TESObjectWEAP* weapon);

	bool __fastcall CheckIfNotMelee(TESObjectWEAP* weapon);

	template <UInt32 retn1, UInt32 retn2> __declspec(naked) void ExecuteAttackHook2() {
		static const auto retnAddr1 = retn1;
		static const auto retnAddr2 = retn2;
		static const auto check = CheckIfNotMelee;
		__asm {
			mov ecx, [ebp - 0xB0]
			call check
			test al, al
			jz jumptwo
			jmp retnAddr1
		jumptwo :
			jmp retnAddr2
		}
	}

	bool __fastcall CheckIfMeleeWithProjectile(TESObjectWEAP* weapon, Actor* actor);

	template <UInt32 retn1, UInt32 retn2> __declspec(naked) void ShouldFireHook() {
		static const auto retnAddr1 = retn1;
		static const auto retnAddr2 = retn2;
		static const auto check = CheckIfMeleeWithProjectile;
		__asm {
			jz skip
			mov ecx, [ebp - 0x24]
			mov edx, [ebp - 0xB4]
			call check
			test al, al
			jnz skip
			jmp retnAddr1
		skip :
			jmp retnAddr2
		}
	}

	bool __fastcall CheckIfMeleePlayerAttack(TESObjectWEAP* weapon);

	template <UInt32 retn1, UInt32 retn2> __declspec(naked) void PlayerAttackRemoveAmmoCheckHook() {
		static const auto retnAddr1 = retn1;
		static const auto retnAddr2 = retn2;
		static const auto check = CheckIfMeleePlayerAttack;
		__asm {
			mov ecx, [ebp - 0x24]
			call check
			test al, al
			jz jumptwo
			jmp retnAddr1
		jumptwo :
			jmp retnAddr2
		}
	}
}
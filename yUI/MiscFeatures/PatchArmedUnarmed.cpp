#include <main.h>

#include <Safewrite.hpp>
#include <SimpleINILibrary.h>

// TODO: evaluate what is the state of effects, what needs to be done
#if 0
namespace Patch::ArmedUnarmed
{
	inline bool g_ArmedUnarmed = false;

	void HandleINIs()
	{
		const auto iniPath = GetCurPath() / yUI_INI;
		CSimpleIniA ini;
		ini.SetUnicode();


		if (ini.LoadFile(iniPath.c_str()) == SI_FILE) return;

		g_ArmedUnarmed = ini.GetOrCreate("General", "bArmedUnarmed", 1, "; Allow for Melee and Unarmed weapons to use ammo and shoot projectiles");

		ini.SaveFile(iniPath.c_str(), false);
	}

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

	bool __fastcall ShouldNotShowAmmo(TESObjectWEAP* weapon)
	{
		const auto ammoform = DYNAMIC_CAST(weapon, TESObjectWEAP, BGSAmmoForm);
		return !(ammoform && ammoform->ammo);
	}

	bool __fastcall QueueAttackFireCheck(TESObjectWEAP* weapon, Actor* actor)
	{
		if (!weapon || !weapon->IsMeleeWeapon()) return true;

		if (!weapon->HasScope()) return false;

		if (!weapon->ammo.ammo)
		{
			if (weapon->projectile) actor->baseProcess->SetQueuedIdleFlag(kIdleFlag_FireWeapon);
		}
		else if (const auto ammoinfo = actor->baseProcess->GetAmmoInfo(); ammoinfo && ammoinfo->ammo)
		{
			if (weapon->projectile)
			{
				if (ammoinfo->countDelta >= weapon->ammoUse) actor->baseProcess->SetQueuedIdleFlag(kIdleFlag_FireWeapon);
			}
			else
			{
				if (ammoinfo->countDelta >= weapon->ammoUse)
				{
					actor->DecreaseClipAmmo(weapon->ammoUse);
					return true;
				}
				else
				{
					actor->ReloadAlt(weapon, 1, 0, 0);
				}
			}
		}


		return false;
	}


	void __fastcall QueuePowerAttackFireCheck(TESObjectWEAP* weapon, Actor* actor)
	{
		if (!weapon || !weapon->IsMeleeWeapon()) return;

		if (!weapon->ammo.ammo)
		{
			if (weapon->projectile) actor->baseProcess->SetQueuedIdleFlag(kIdleFlag_FireWeapon);
		}
		else if (const auto ammoinfo = actor->baseProcess->GetAmmoInfo(); ammoinfo && ammoinfo->ammo)
		{
			if (weapon->projectile)
			{
				if (ammoinfo->countDelta >= weapon->ammoUse) actor->baseProcess->SetQueuedIdleFlag(kIdleFlag_FireWeapon);
			}
			else
			{
				if (ammoinfo->countDelta >= weapon->ammoUse)
				{
					actor->DecreaseClipAmmo(weapon->ammoUse);
				}
				else
				{
					actor->ReloadAlt(weapon, 1, 0, 0);
				}
			}
		}
	}

	bool __fastcall ExecuteAttackHook(TESObjectWEAP* weapon)
	{
		if (!weapon->IsMeleeWeapon()) return true;
		if (weapon->projectile) return true;
		return false;
	}

	bool __fastcall CheckIfNotMelee(TESObjectWEAP* weapon)
	{
		return !weapon->IsMeleeWeapon();
	}

	bool __fastcall CheckIfMeleeWithProjectile(TESObjectWEAP* weapon, Actor* actor)
	{
		if (weapon->IsMeleeWeapon() && weapon->projectile) return true;
		return false;
	}

	bool __fastcall CheckIfMeleePlayerAttack(TESObjectWEAP* weapon)
	{
		if (!weapon) return false;
		if (weapon->IsMeleeWeapon() && !weapon->IsAutomatic()) return false;
		return true;
	}

	void patchArmedUnarmed(const bool bEnable)
	{
		if (bEnable) {
			WriteRelCall(0x7724CB, ShouldNotShowAmmo);

			WriteRelJump(0x893C37, ShouldFireHook<0x893C3D, 0x893E2C>);

			WriteRelJump(0x895C9B, QueuePowerAttackFireHook<0x895CA1>);
			WriteRelJump(0x895D39, QueueAttackFireHook<0x895D3E>);
			WriteRelCall(0x8BADD3, ExecuteAttackHook);
			WriteRelJump(0x8BAE53, ExecuteAttackHook2<0x8BAE58, 0x8BAF03>);

			WriteRelJump(0x9492A1, PlayerAttackRemoveAmmoCheckHook<0x949308, 0x94948D>);
		}
	}

	extern void Init()
	{
		if (g_nvseInterface->isEditor) return;
		HandleINIs();
	}

}
#endif
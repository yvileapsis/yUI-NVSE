#include <main.h>
#include <SafeWrite.h>
#include <SimpleINILibrary.h>

#include "GameProcess.h"
#include "GameObjects.h"
#include "GameExtraData.h"
#include "GameRTTI.h"

namespace Fix::ProjectileEffects
{
	inline int g_CorrectAmmoEffects = 0;
	inline int g_CorrectMeltdownEffects = 0;
	inline int g_CorrectWeaponEffects = 0;

	void HandleINIs()
	{
		CSimpleIniA ini;
		ini.SetUnicode();
		const auto iniPath = GetCurPath() + yUI_INI;
		if (ini.LoadFile(iniPath.c_str()) == SI_FILE) return;

		g_CorrectAmmoEffects = ini.GetOrCreate("Projectile Bugfixes", "bCorrectAmmoEffects", 0, "; EXPERIMENTAL DO NOT TOUCH");
		g_CorrectMeltdownEffects = ini.GetOrCreate("Projectile Bugfixes", "bCorrectMeltdownEffects", 0, "; EXPERIMENTAL DO NOT TOUCH");
		g_CorrectWeaponEffects = ini.GetOrCreate("Projectile Bugfixes", "bCorrectWeaponEffects", 0, "; EXPERIMENTAL DO NOT TOUCH");

		ini.SaveFile(iniPath.c_str(), false);
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

	TESObjectWEAP* __fastcall EffectGetWeapon(InventoryChanges* entry, Projectile* projectile);
	EnchantmentItem* __fastcall EffectGetEnchantment(InventoryChanges* entry, Projectile* projectile);
	AlchemyItem* __fastcall EffectGetPoison(InventoryChanges* entry, Projectile* projectile);

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

	TESObjectWEAP* __fastcall EffectGetWeapon(InventoryChanges* entry, Projectile* projectile)
	{
		if (projectile && projectile->weapon) return projectile->weapon;
		return reinterpret_cast<TESObjectWEAP*>(entry->form);
	}

	EnchantmentItem* __fastcall EffectGetEnchantment(InventoryChanges* entry, Projectile* projectile)
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

	AlchemyItem* __fastcall EffectGetPoison(InventoryChanges* entry, Projectile* projectile)
	{
		if (!entry) return nullptr;
		auto const one = reinterpret_cast<ExtraPoison*>(entry->GetExtraData(kExtraData_Poison));
		return one ? one->poisonEffect : nullptr;
	}

	char __fastcall MergeScriptEvent(const ActorHitData* a0)
	{
		if (!a0 || !a0->source || !a0->source->baseProcess) return 0;
		const auto weaponinfo = reinterpret_cast<MiddleHighProcess*>(a0->source->baseProcess)->weaponInfo;
		auto a2 = weaponinfo ? weaponinfo->GetExtraData() : nullptr;

		if (!a2) a2 = ExtraDataList::Create();

		const auto newScript = DYNAMIC_CAST(a0->GetWeapon(), TESObjectWEAP, TESScriptableForm);

		if (!newScript || !newScript->script) return 0;

		auto xData = a2->GetByType(kExtraData_Script);

		if (const auto xScript = DYNAMIC_CAST(xData, BSExtraData, ExtraScript)) {
			xScript->script = newScript->script;

			/*		if (xScript->eventList)
						xScript->eventList->m_script = newScript->script;
					if (xScript->eventList->m_eventList)
						for (const auto i : *xScript->eventList->m_eventList)
							i->object = a0->GetWeapon();*/
		}
		else {
			a2->Add(ExtraScript::Create(newScript, true, nullptr));
		}

		/*	if (!weaponinfo)
			{
				auto contchanges = InventoryChanges::Create(a0->GetWeapon(), 1, nullptr);
				contchanges->Add(a2);
				reinterpret_cast<MiddleHighProcess*>(a0->source->baseProcess)->weaponInfo = (BaseProcess::WeaponInfo*)contchanges;
				weaponinfo = reinterpret_cast<MiddleHighProcess*>(a0->source->baseProcess)->weaponInfo;
			}*/

		const auto ret = CdeclCall<char>(0x5AC750, a0->target, a2, 0x80);

		return ret;
	}

	void __fastcall StoreAmmoOnProjectile(Projectile* projectile)
	{
		const auto process = reinterpret_cast<Actor*>(projectile->source)->baseProcess;
		if (!process || !process->GetAmmoInfo()) return;
		projectile->ammo = process->GetAmmoInfo()->ammo;
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
		if (explosion->CanStoreAmmo()) return false;
		if (!explosion->ammo) return false;
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

		if (!process->GetWeaponInfo() || !process->weaponInfo || !process->weaponInfo->weapon) return;
		g_weapon.emplace(process, process->weaponInfo->weapon);
		if (const auto weapon = hitData->GetWeapon()) process->weaponInfo->weapon = weapon;

		if (!process->GetAmmoInfo() || !process->ammoInfo || !process->ammoInfo->ammo) return;
		g_ammo.emplace(process, process->ammoInfo->ammo);
		if (const auto ammo = hitData->GetAmmo()) process->ammoInfo->ammo = ammo;
	}

	void PostCalculateHitDamage(ActorHitData* hitData)
	{
		if (!hitData || !hitData->source || !hitData->source->baseProcess) return;
		const auto process = reinterpret_cast<MiddleHighProcess*>(hitData->source->baseProcess);

		if (!process->GetWeaponInfo() || !process->weaponInfo || !process->weaponInfo->weapon) return;
		if (g_weapon.contains(process))
		{
			process->weaponInfo->weapon = g_weapon[process];
			g_weapon.erase(process);
		}

		if (!process->GetAmmoInfo() || !process->ammoInfo || !process->ammoInfo->ammo) return;
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

	void patchCorrectAmmoEffects(const bool bEnable)
	{
		if (bEnable) {

			SafeWriteBuf(0x9BC0C9, "\xD9\xEE", 2);								// zero the health percent

			WriteRelJump(0x9BC1D7, StoreAmmoOnProjectileHook<0x9BC1F9>);		// store ammo on the projectile

			WriteRelJump(0x9C36E3, StoreAmmoOnExplosionHook<0x9C36F1>);			// copy ammo from projectile to explosion

			WriteRelJump(0x89ABDC, GetAmmoFromHitDataHook<0x89AC3C>);			// get ammo from projectile and explosion


			WriteRelJump(0x9AC77E, FixExplosionAmmoHook1<0x9AC785>);			// HACKY HACKY HACKY
			WriteRelJump(0x9B2662, FixExplosionAmmoHook1<0x9B2669>);			// Fix Destruct and Save so that it checks for 
			WriteRelJump(0x9B066E, FixExplosionAmmoHook1<0x9B0675>);
			WriteRelJump(0x9B21F7, FixExplosionAmmoHook2<0x9B2201>);			// the same things Init does
			WriteRelJump(0x9B0681, FixExplosionAmmoHook3<0x9B0688>);
			// jipln

			WriteRelJump(0x9B5623, PreCalculateHitDamageHook<0x9B5628>);
			WriteRelCall(0x9B562D, PostCalculateHitDamageHook1);
			WriteRelJump(0x9B5702, PreCalculateHitDamageHook<0x9B5707>);
			WriteRelCall(0x9B5716, PostCalculateHitDamageHook2);
			WriteRelJump(0x9B575C, PreCalculateHitDamageHook<0x9B5761>);
			WriteRelCall(0x9B5764, PostCalculateHitDamageHook2);
			WriteRelJump(0x9B58AE, PreCalculateHitDamageHook<0x9B58B3>);
			WriteRelCall(0x9B58BE, PostCalculateHitDamageHook2);
			WriteRelJump(0x9B5A10, PreCalculateHitDamageHook<0x9B5A15>);
			WriteRelCall(0x9B5A18, PostCalculateHitDamageHook2);
		}
	}

	void patchCorrectMeltdownEffects(const bool bEnable)
	{
		if (bEnable) {
			WriteRelJump(0x89B455, MeltdownPassHitDataWeaponHook<0x89B4CC>);	// pass current weapon not player weapon
			WriteRelJump(0x89B670, MeltdownSetWeaponAndAmmoHook<0x89B6D0>);		// set weapon and ammo for the meltdown explosion
		}
	}

	void patchCorrectWeaponEffects(const bool bEnable)
	{
		if (bEnable) {

			//	WriteRelCall(0x8C248E, Test);
			//	WriteRelCall(0x8C2593, Test);
			//	WriteRelCall(0x8C250E, Test);
			//	WriteRelJump(0x816266, Test3);


			WriteRelJump(0x8C2243, WeaponEffectHook1<0x8C2310>);
			WriteRelJump(0x8C2342, WeaponEffectHook2<0x8C2366>);

			WriteRelJump(0x89AB44, MergeScriptEventHook<0x89ABDC>);
		}
	}

/*
void __fastcall DoFireWeaponEx(TESObjectREFR* refr, int EDX, TESObjectWEAP* weapon, TESAmmo* ammo, NiNode* node)
{
	const auto highProcess = static_cast<HighProcess*>(static_cast<Actor*>(refr)->baseProcess);

	TESObjectWEAP* oldWeapon = nullptr;
	if (weapon && highProcess->weaponInfo)
	{
		oldWeapon = highProcess->weaponInfo->weapon;
		highProcess->weaponInfo->weapon = weapon;
	}

	TESAmmo* oldAmmo = nullptr;
	if (ammo && highProcess->ammoInfo)
	{
		oldAmmo = highProcess->ammoInfo->ammo;
		highProcess->ammoInfo->ammo = ammo;
	}

	NiNode* oldNode = nullptr;
	if (node) {
		oldNode = highProcess->projectileNode;
		highProcess->projectileNode = node;
	}

	ThisCall<void>(0x523150, weapon, refr);

	if (oldNode) highProcess->projectileNode = oldNode;
	if (oldWeapon) highProcess->weaponInfo->weapon = oldWeapon;
	if (oldAmmo) highProcess->ammoInfo->ammo = oldAmmo;
}

extern inline bool Cmd_FireWeaponAlt_Execute(COMMAND_ARGS)
{
	TESObjectWEAP* weapon = nullptr;
	TESAmmo* ammo = nullptr;
	UInt32 mods = 0;
	Float32 condition = 0;
	char nodeName[0x40] = "\0";
	if (!(thisObj->IsActor() && ExtractArgsEx(EXTRACT_ARGS_EX, &weapon, &nodeName, &ammo, &mods, &condition) && weapon->typeID == kFormType_TESObjectWEAP && weapon->projectile)) return true;

	const auto node = nodeName[0] ? thisObj->GetNode(nodeName) : nullptr;

	if (CdeclCall<bool>(0x8C7AA0))
	{
		ScrapHeapQueue::QueuedCmdCall qCall(DoFireWeaponEx, thisObj->refID, 1);
		qCall.args[0] = weapon;
		qCall.args[1] = ammo;
		qCall.args[2] = node;
		ScrapHeapQueue::GetSingleton()->AddQueuedCmdCall(qCall);
	}
	else DoFireWeaponEx(thisObj, 0, weapon, ammo, node);

	return true;
}
*/

	extern void Init()
	{
		if (g_nvseInterface->isEditor) return;
		HandleINIs();
		patchCorrectAmmoEffects(g_CorrectAmmoEffects);
		patchCorrectWeaponEffects(g_CorrectWeaponEffects);
		patchCorrectMeltdownEffects(g_CorrectMeltdownEffects);
	}
}
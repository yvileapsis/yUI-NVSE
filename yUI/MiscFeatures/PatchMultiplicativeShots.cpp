#include <main.h>

#include <SafeWrite.h>
#include <SimpleINILibrary.h>
#include <Forms.h>
#include <GameRTTI.h>

namespace Patch::MultiplicativeShots
{
	void HandleINIs()
	{
		const auto iniPath = GetCurPath() + yUI_INI;
		CSimpleIniA ini;
		ini.SetUnicode();


		if (ini.LoadFile(iniPath.c_str()) == SI_FILE) return;

		ini.SaveFile(iniPath.c_str(), false);
	}

	UInt8 __fastcall TESObjectWEAPGetNumProjectilesHook(TESObjectWEAP* weapon, void* dummyEdx, char hasWeaponMod, char dontCheckAmmo, TESForm* form);

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

	void patchMultiplicativeProjectileCount(const bool bEnable)
	{
		if (bEnable) {
			WriteRelJump(0x525B20, TESObjectWEAPGetNumProjectilesHook);
			SafeWrite8(0x708343, 0x38);
			WriteRelCall(0x70835E, 0x525B20); // undo stewie's fix, replace with mine
		}
	}

	extern void Init()
	{
		if (g_nvseInterface->isEditor) return;
		HandleINIs();
		patchMultiplicativeProjectileCount(true);
	}
}

#include <main.h>
#include <GameSettings.h>
#include <GameProcess.h>

#include <SafeWrite.h>
#include <SimpleINILibrary.h>

#include "TESObjectREFR.h"

namespace Patch::RestoreFO3Spread
{
	inline bool g_AlterSpread = false;

	void HandleINIs()
	{
		CSimpleIniA ini;
		ini.SetUnicode();

		const auto iniPath = GetCurPath() + yUI_INI;
		if (ini.LoadFile(iniPath.c_str()) == SI_FILE) return;

		g_AlterSpread = ini.GetOrCreate("General", "iAlterSpread", 1, "; restore gamesettings controlling the spread value on weapon forms.");

		ini.SaveFile(iniPath.c_str(), false);
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

	bool AlterSpread()
	{
		return g_AlterSpread;
	}

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
			fstp[ebp - 0x24]

			jmp retnAddr2

		no :
			movzx ecx, [ebp + 0x18]
			test ecx, ecx
			jmp retnAddr1
		}
	}

	Float32 __fastcall GetMinSpread(Actor* actor) {
		const auto conditionTier = ThisStdCall<SInt32>(0x92B9E0, actor->baseProcess, actor);
		if (conditionTier > 9) return 0.5;
		const auto spreadArray = reinterpret_cast<Setting**>(0x119B2BC);
		const Setting* spreadSetting = spreadArray[conditionTier + 10];
		const float result = spreadSetting->data.f;
		actor->spreadWeapon = result;
		return result;
	}


	Float64 __fastcall AlterSpreadCalculation(Actor* actor, TESObjectWEAP* weapon, Float32 condition, char isAiming,
		char isSneaking, char isWalking, char isRunning)
	{
		const auto IronSightsBonus = GameSettingFromString("fGunSpreadIronSightsBase")->data.f + isAiming * GameSettingFromString("fGunSpreadIronSightsMult")->data.f;
		const auto CrouchBonus = GameSettingFromString("fGunSpreadCrouchBase")->data.f + isSneaking * GameSettingFromString("fGunSpreadCrouchMult")->data.f;
		const auto ConditionPenalty = GameSettingFromString("fGunSpreadCondBase")->data.f + condition * GameSettingFromString("fGunSpreadCondMult")->data.f;
		const auto SkillBonus = 0; // GameSettingFromString("fGunSpreadSkillBase")->data.f + wah * GameSettingFromString("fGunSpreadSkillMult")->data.f;
		const auto WalkPenalty = GameSettingFromString("fGunSpreadWalkBase")->data.f + isWalking * GameSettingFromString("fGunSpreadWalkMult")->data.f;
		const auto RunPenalty = GameSettingFromString("fGunSpreadRunBase")->data.f + isRunning * GameSettingFromString("fGunSpreadRunMult")->data.f;
		const auto ArmPenalty = 0; // GameSettingFromString("fGunSpreadArmBase")->data.f + wah * GameSettingFromString("fGunSpreadArmMult")->data.f;
	//	const auto NPCArmPenalty = GameSettingFromString("fGunSpreadNPCArmBase")->data.f + wah * GameSettingFromString("fGunSpreadNPCArmMult")->data.f;
	//	const auto HeadPenalty = GameSettingFromString("fGunSpreadHeadBase")->data.f + wah * GameSettingFromString("fGunSpreadHeadMult")->data.f;

		return  (IronSightsBonus * CrouchBonus * (ConditionPenalty + SkillBonus) * (WalkPenalty + RunPenalty) + ArmPenalty);
	}

	Float64 __fastcall GetSpreadCondition(Actor* actor)
	{
		if (actor->spreadHealthPercent < 0.0)
		{
			const auto entry = actor->baseProcess->GetWeaponInfo();
			actor->spreadHealthPercent = entry->GetHealthPercent(1) / 100.0;
		}
		return actor->spreadHealthPercent;
	}

	void patchRestoreSpreadGameSettings(const bool bEnable)
	{
		if (bEnable) {

			WriteRelJump(0x8B0E54, RestoreMinSpreadHook<0x8B0E65>);
			WriteRelJump(0x8B0EEE, RestoreSpreadConditionHook<0x8B0F01>);
			WriteRelJump(0x64695B, AlterSpreadHook<0x646961, 0x646CA3>);
		}
		else {

			UndoSafeWrite(0x8B0E54);
			UndoSafeWrite(0x8B0EEE);
			UndoSafeWrite(0x64695B);
		}
	}

	extern void Init()
	{
		if (g_nvseInterface->isEditor) return;
		HandleINIs();
		patchRestoreSpreadGameSettings(g_AlterSpread);
	}
}

#include <main.h>
#include <SafeWrite.h>

#include <SimpleINILibrary.h>

#include <GameRTTI.h>
#include <TESObjectREFR.h>

namespace Patch::ExplosionForce
{
	inline bool enable = true;

	Float64 __cdecl AdjustPushForceAlt(Actor* target, ActorHitData* hitdata, ActorValueOwner* owner, SInt32 force)
	{
		Float64 scale = 1.0;
		if (hitdata && hitdata->explosion && hitdata->explosion->IsExplosion() && hitdata->explosion->baseForm)
		{
			const auto explosion = DYNAMIC_CAST(hitdata->explosion->baseForm, TESForm, BGSExplosion);
			scale = CdeclCall<Float64>(0x647920, hitdata->explosion->radius, hitdata->explosion->GetDistance(target));
			scale *= explosion->force / 100.0;
		}
		return scale * CdeclCall<Float64>(0x646580, owner, force);
	}

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

	void PatchFixExplosionPushForce(const bool enable)
	{
		if (enable) { // stooee
			WriteRelJump(0x89C8CF, HitKnockbackHook<0x89C8D4>);
			SafeWriteBuf(0x89C8E3, "\x41\x08\x85\xC0\x0F\x44\xC2\x83\xC0\x30\x0F\x1F\x40\x00", 14);
		} else {
			UndoSafeWrite(0x89C8CF);
			UndoSafeWrite(0x89C8E3);
		}
	}

	void HandleINIs()
	{
		const auto iniPath = GetCurPath() + yUI_INI;
		CSimpleIniA ini;
		ini.SetUnicode();


		if (ini.LoadFile(iniPath.c_str()) == SI_FILE) return;

		enable = ini.GetOrCreate("General", "bFixExplosionPushForce", 1, "; fix force of the explosions to scale both with distance (like explosion damage) and with actual force of the explosion's baseform.");

		ini.SaveFile(iniPath.c_str(), false);
	}

	extern void Init()
	{
		if (g_nvseInterface->isEditor) return;
		HandleINIs();
		PatchFixExplosionPushForce(enable);
	}
}

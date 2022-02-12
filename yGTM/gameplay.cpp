#include <gameplay.h>
#include <GameObjects.h>
#include <GameRTTI.h>

double __cdecl AdjustPushForceAlt(Actor* target, ActorHitData* hitdata, ActorValueOwner* owner, int force)
{
	double scale = 1.0;
	if (hitdata->explosion && hitdata->explosion->baseForm)
	{
		const auto explosion = DYNAMIC_CAST(hitdata->explosion->baseForm, TESForm, BGSExplosion);
		scale = 1 - hitdata->explosion->GetDistance(target) / hitdata->explosion->radius;
		if (scale < 0) scale = 0;
		scale *= scale;
		scale *= explosion->force / 100.0;
	}
	return scale * CdeclCall<double>(0x646580, owner, force);
}

__declspec(naked) void HitKnockbackHook()
{
	static const UInt32 retnAddr = 0x89C8D4;
	static const UInt32 AdjustPushForce = reinterpret_cast<UInt32>(AdjustPushForceAlt);
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
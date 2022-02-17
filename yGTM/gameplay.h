#pragma once
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

Float32 __fastcall SetSpreadForActor(Actor* actor);

template <UInt32 retn> __declspec(naked) void RestoreSpreadHook() {
	static const auto retnAddr = retn;
	static const auto SpreadFunc = SetSpreadForActor;
	__asm {
		call SpreadFunc
		fstp[ebp - 0x10]
		jmp retnAddr
	}
}
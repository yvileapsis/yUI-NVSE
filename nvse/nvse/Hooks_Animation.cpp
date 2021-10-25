#include "Hooks_Animation.h"
#include "SafeWrite.h"
#include <cstdarg>
#include "Utilities.h"
#include "GameForms.h"

#define kHookGetGlobalModelPath 0x0104A1B8

static __declspec(naked) char* AnimationHook(void)
{
	_asm {
		mov eax, [ecx + 4]
		retn
	}
}

void Hook_Animation_Init(void)
{
	UInt32	enableAnimationHook = 0;

	if(GetNVSEConfigOption_UInt32("Animation", "EnableAnimationHook", &enableAnimationHook) && enableAnimationHook)
	{
		SafeWrite32(kHookGetGlobalModelPath, (UInt32)AnimationHook);
	}
}
#include <gameplay.h>
#include <GameObjects.h>
#include <GameRTTI.h>

#include "GameSettings.h"

Float64 __cdecl AdjustPushForceAlt(Actor* target, ActorHitData* hitdata, ActorValueOwner* owner, SInt32 force)
{
	Float64 scale = 1.0;
	if (hitdata->explosion && hitdata->explosion->baseForm)
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
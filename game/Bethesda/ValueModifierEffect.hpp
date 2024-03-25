#pragma once
#include "ActiveEffect.hpp"
#include "ActorValue.hpp"

// 0x4C
class ValueModifierEffect : public ActiveEffect
{
public:
	ValueModifierEffect();
	~ValueModifierEffect();

	EnumActorValueCode actorVal;	// 48
};
static_assert(sizeof(ValueModifierEffect) == 0x04C);
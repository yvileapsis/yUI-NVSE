#pragma once
#include "ActiveEffect.hpp"

// 0x4C
class AssociatedItemEffect : public ActiveEffect
{
public:
	AssociatedItemEffect();
	~AssociatedItemEffect();

	TESObject* pkItem;	// 48 - creature, armor, weapon
};
static_assert(sizeof(AssociatedItemEffect) == 0x4C);
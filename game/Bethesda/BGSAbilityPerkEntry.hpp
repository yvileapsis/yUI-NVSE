#pragma once
#include "BGSPerkEntry.hpp"

class SpellItem;

class BGSAbilityPerkEntry : public BGSPerkEntry
{
public:
	BGSAbilityPerkEntry();
	~BGSAbilityPerkEntry();

	SpellItem* pkAbility;			// 08
};
static_assert(sizeof(BGSAbilityPerkEntry) == 0xC);
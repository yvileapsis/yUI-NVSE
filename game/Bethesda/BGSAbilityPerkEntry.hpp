#pragma once

#include "BGSPerkEntry.hpp"

class SpellItem;

class BGSAbilityPerkEntry : public BGSPerkEntry {
public:
	BGSAbilityPerkEntry();
	~BGSAbilityPerkEntry();

	SpellItem* ability;			// 08
};
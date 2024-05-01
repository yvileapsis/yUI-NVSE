#pragma once
#include "BSExtraData.hpp"

class TESCombatStyle;

// 0x10
class ExtraCombatStyle : public BSExtraData {
public:
	ExtraCombatStyle();
	virtual ~ExtraCombatStyle();

	TESCombatStyle* combatStyle;		// 00C
};
static_assert(sizeof(ExtraCombatStyle) == 0x10);
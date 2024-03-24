#pragma once

#include "BSExtraData.hpp"

class TESCombatStyle;

class ExtraCombatStyle : public BSExtraData {
public:
	ExtraCombatStyle();
	virtual ~ExtraCombatStyle();

	TESCombatStyle* combatStyle;		// 00C
};
#pragma once

#include "tList.hpp"

class CombatController;

class CombatAction {
public:
	virtual void	IsApplicable();
	virtual void	AddProcedureToController(CombatController* combatCtrl, void* a2);
	virtual void	Unk_02();
	virtual void	GetCost(CombatController* combatCtrl, int a2);

	tList<void>		list04;		// 04
	tList<void>		list0C;		// 0C
	tList<void>		list14;		// 14
	tList<void>		list1C;		// 1C
	UInt32			actionID;	// 24
	UInt32			unk28;		// 28
};

ASSERT_SIZE(CombatAction, 0x2C);
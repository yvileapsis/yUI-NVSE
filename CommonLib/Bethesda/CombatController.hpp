#pragma once

#include "TESPackage.hpp"
#include "BSSimpleArray.hpp"
#include "CombatAction.hpp"
#include "CombatProcedure.hpp"
#include "TESCombatStyle.hpp"

class CombatActors;
class CombatGoal;
class TESObjectWEAP;
class PathingCoverLocation;
class PathingCoverLocation;
class UnreachableCoverLocation;
class UnreachableLocation;
class Actor;

enum CombatActions {
	COMBAT_ACTION_ATTACK_RANGED_EXPLOSIVE,
	COMBAT_ACTION_ATTACK_RANGED_EXPLOSIVE_F,
	COMBAT_ACTION_ATTACK_RANGED,
	COMBAT_ACTION_ATTACK_RANGED_FROM_COVER,
	COMBAT_ACTION_ATTACK_GRENADE,
	COMBAT_ACTION_ATTACK_GRENADE_FLUSH_TARG,
	COMBAT_ACTION_ATTACK_GRENADE_FROM_COVER,
	COMBAT_ACTION_ATTACK_MELEE,
	COMBAT_ACTION_ATTACK_HAND_TO_HAND,
	COMBAT_ACTION_MOVE,
	COMBAT_ACTION_MOVE_AND_SWITCH_TO_MELEE,
	COMBAT_ACTION_MOVE_AND_ATTACK_RANGED,
	COMBAT_ACTION_MOVE_AND_ATTACK_RANGED_EX,
	COMBAT_ACTION_MOVE_AND_ATTACK_GRENADE,
	COMBAT_ACTION_DRAW_WEAPON,
	COMBAT_ACTION_SWITCH_WEAPON,
	COMBAT_ACTION_AVOID_THREAT,
	COMBAT_ACTION_SEARCH,
	COMBAT_ACTION_INVESTIGATE,
	COMBAT_ACTION_DODGE,
	COMBAT_ACTION_IGNORE_BLOCKED_TARGET,
	COMBAT_ACTION_FLEE,
	COMBAT_ACTION_ACTIVATE_COMBAT_ITEM,
	COMBAT_ACTION_USE_COMBAT_ITEM,
	COMBAT_ACTION_ACQUIRE_LINE_OF_SIGHT,
	COMBAT_ACTION_HIDE,
	COMBAT_ACTION_APPROACH_TARGET
};

class CombatController : public TESPackage {
public:
	struct Unk09C
	{
		UInt32									unk000[4];	// 000
		TESObjectWEAP* weapon1;	// 010
		TESObjectWEAP* weapon2;	// 014
		TESObjectWEAP* weapon3;	// 018
		TESObjectWEAP* weapon4;	// 01C
		UInt32									unk020;		// 020
		BSSimpleArray<TESObjectWEAP>			arr024;		// 024
		UInt32									unk034[24];	// 034
		float									flt094;		// 094
		UInt32									unk098[11];	// 098
		void* ptr0C4;	// 0C4
		UInt32									unk0C8[17];	// 0C8
		BSSimpleArray<PathingCoverLocation>		arr10C;		// 10C
		UInt32									unk11C[11];	// 11C
		BSSimpleArray<PathingCoverLocation>		arr148;		// 148
		UInt32									unk158[3];	// 158
		BSSimpleArray<UnreachableCoverLocation>	arr164;		// 164
		BSSimpleArray<UnreachableLocation>		arr174;		// 174
		UInt32									unk184[15];	// 184
		Actor* actor1C0;	// 1C0
		CombatController* cmbtCtrl;	// 1C4
		UInt32									unk1C8[25];	// 1C8
	};
	ASSERT_SIZE(Unk09C, 0x22C);

	CombatActors* combatActors;		// 080
	CombatProcedure* combatProcedure1;	// 084
	CombatProcedure* combatProcedure2;	// 088
	BSSimpleArray<CombatProcedure>	combatProcedures;	// 08C
	Unk09C* struct09C;			// 09C
	void* ptr0A0;			// 0A0
	UInt32							unk0A4;				// 0A4
	CombatAction* combatAction;		// 0A8
	CombatGoal* combatGoal;		// 0AC
	UInt32							unk0B0;				// 0B0
	float							flt0B4[2];			// 0B4
	Actor* packageOwner;		// 0BC
	Actor* packageTarget;		// 0C0
	UInt32							unk0C4[2];			// 0C4
	float							flt0CC;				// 0CC
	float							flt0D0;				// 0D0
	UInt8							byte0D4;			// 0D4
	UInt8							byte0D5;			// 0D5
	UInt8							pad0D6[2];			// 0D6
	float							flt0D8;				// 0D8
	float							flt0DC;				// 0DC
	float							flt0E0;				// 0E0
	UInt32							unk0E4[3];			// 0E4
	TESObjectWEAP* weapon;			// 0F0
	TESCombatStyle* combatStyle;		// 0F4
	UInt32							unk0F8[11];			// 0F8
	UInt8							byte124;			// 124
	bool							stopCombat;			// 125
	UInt8							byte126;			// 126
	UInt8							byte127;			// 127
	UInt32							unk128[8];			// 128
	float							flt148;				// 148
	UInt32							unk14C[15];			// 14C
};

ASSERT_SIZE(CombatController, 0x188);
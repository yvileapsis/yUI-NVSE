#pragma once
#include "TESForm.hpp"

// 0x108
class TESCombatStyle : public TESForm
{
public:
	TESCombatStyle();
	~TESCombatStyle();

	enum
	{
		kFlag_ChooseAttackUsingChance		= 1 << 0,
		kFlag_MeleeAlertOK					= 1 << 1,
		kFlag_FleeBasedOnPersonalSurvival	= 1 << 2,

		kFlag_IgnoreThreats					= 1 << 4,
		kFlag_IgnoreDamagingSelf			= 1 << 5,
		kFlag_IgnoreDamagingGroup			= 1 << 6,
		kFlag_IgnoreDamagingSpectators		= 1 << 7,
		kFlag_CannotUseStealthboy			= 1 << 8,
	};


	Float32		coverSearchRadius;				// 018
	Float32		takeCoverChance;				// 01C
	Float32		waitTimeMin;					// 020
	Float32		waitTimeMax;					// 024
	Float32		waitToFireTimerMin;				// 028
	Float32		waitToFireTimerMax;				// 02C
	Float32		fireTimerMin;					// 030
	Float32		fireTimerMax;					// 034
	Float32		rangedWeapRangeMultMin;			// 038
	UInt8		pad3C[4];						// 03C
	UInt8		weaponRestrictions;				// 040
	UInt8		pad41[3];						// 041
	Float32		rangedWeapRangeMultMax;			// 044
	Float32		maxTargetingFOV;				// 048
	Float32		combatRadius;					// 04C
	Float32		semiAutoFiringDelayMultMin;		// 050
	Float32		semiAutoFiringDelayMultMax;		// 054
	UInt8		dodgeChance;					// 058
	UInt8		LRChance;						// 059
	UInt8		pad5A[2];						// 05A
	Float32		dodgeLRTimerMin;				// 05C
	Float32		dodgeLRTimerMax;				// 060
	Float32		dodgeFWTimerMin;				// 064
	Float32		dodgeFWTimerMax;				// 068
	Float32		dodgeBKTimerMin;				// 06C
	Float32		dodgeBKTimerMax;				// 070
	Float32		idleTimerMin;					// 074
	Float32		idleTimerMax;					// 078
	UInt8		blockChance;					// 07C
	UInt8		attackChance;					// 07D
	UInt8		pad7E[2];						// 07E
	Float32		staggerBonusToAttack;			// 080
	Float32		KOBonusToAttack;				// 084
	Float32		H2HBonusToAttack;				// 088
	UInt8		powerAttackChance;				// 08C
	UInt8		pad8D[3];						// 08D
	Float32		staggerBonusToPower;			// 090
	Float32		KOBonusToPower;					// 094
	UInt8		powerAttackN;					// 098
	UInt8		powerAttackF;					// 099
	UInt8		powerAttackB;					// 09A
	UInt8		powerAttackL;					// 09B
	UInt8		powerAttackR;					// 09C
	UInt8		pad9D[3];						// 09D
	Float32		holdTimerMin;					// 0A0
	Float32		holdTimerMax;					// 0A4
	UInt16		csFlags;						// 0A8
	UInt8		pad10A[2];						// 0AA
	UInt8		acrobaticDodgeChance;			// 0AC
	UInt8		rushAttackChance;				// 0AD
	UInt8		pad10E[2];						// 0AE
	Float32		rushAttackDistMult;				// 0B0
	Float32		dodgeFatigueModMult;			// 0B4
	Float32		dodgeFatigueModBase;			// 0B8
	Float32		encumSpeedModBase;				// 0BC
	Float32		encumSpeedModMult;				// 0C0
	Float32		dodgeUnderAttackMult;			// 0C4
	Float32		dodgeNotUnderAttackMult;		// 0C8
	Float32		dodgeBackUnderAttackMult;		// 0CC
	Float32		dodgeBackNotUnderAttackMult;	// 0D0
	Float32		dodgeFWAttackingMult;			// 0D4
	Float32		dodgeFWNotAttackingMult;		// 0D8
	Float32		blockSkillModMult;				// 0DC
	Float32		blockSkillModBase;				// 0E0
	Float32		blockUnderAttackMult;			// 0E4
	Float32		blockNotUnderAttackMult;		// 0E8
	Float32		attackSkillModMult;				// 0EC
	Float32		attackSkillModBase;				// 0F0
	Float32		attackUnderAttackMult;			// 0F4
	Float32		attackNotUnderAttackMult;		// 0F8
	Float32		attackDuringBlockMult;			// 0FC
	Float32		powerAttackFatigueModBase;		// 100
	Float32		powerAttackFatigueModMult;		// 104

	void SetFlag(UInt32 pFlag, bool bMod)
	{
		csFlags = bMod ? (csFlags | pFlag) : (csFlags & ~pFlag);
	}
};
static_assert(sizeof(TESCombatStyle) == 0x108);
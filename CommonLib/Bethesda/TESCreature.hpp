#pragma once
#include "TESActorBase.hpp"
#include "TESAttackDamageForm.hpp"
#include "TESModelList.hpp"

class BGSImpactDataSet;
class BGSListForm;

// 0x160
class TESCreature :
	public TESActorBase,		// 000
	public TESAttackDamageForm,	// 10C
	public TESModelList			// 114
{
public:
	TESCreature();
	~TESCreature();

	// data
	TESCreature*		creatureTemplate;	// 128
	UInt8				type;				// 12C
	UInt8				combatSkill;		// 12D
	UInt8				magicSkill;			// 12E
	UInt8				stealthSkill;		// 12F
	UInt8				attackReach;		// 130
	UInt8				pad0131[3];			// 131
	Float32				turningSpeed;		// 134
	Float32				footWeight;			// 138
	Float32				baseScale;			// 13C
	TESCombatStyle*		combatStyle;		// 140
	BGSBodyPartData*	bodyPartData;		// 144
	UInt32				materialType;		// 148
	BGSImpactDataSet*	impactDataSet;		// 14C
	UInt32				unk0150;			// 150
	UInt32				soundLevel;			// 154
	BGSListForm*		meleeWeaponList;	// 158
	UInt8				byt015C;			// 15C
	UInt8				pad015D[3];			// 15D
};
static_assert(sizeof(TESCreature) == 0x160);
#pragma once
#include "BSString.hpp"
#include "BSSimpleList.hpp"
#include "BSSimpleArray.hpp"
#include "TESCondition.hpp"

class TESObjectREFR;
class TESQuest;

// 0x24
class BGSQuestObjective
{
public:
	BGSQuestObjective();
	virtual ~BGSQuestObjective();

	enum {
		eQObjStatus_displayed = 1,
		eQObjStatus_completed = 2,
	};

	struct TargetData
	{
		TESObjectREFR*	pkTarget;
		UInt8			ucFlags;
		UInt8			ucFiller[3];
	};

	struct ParentSpaceNode;
	struct TeleportLink
	{
		TESObjectREFR*	pkDoor;
		UInt32			unk004[3];
	};	// 016

	struct Target
	{
		UInt8							byt000;				// 000
		UInt8							fill[3];			// 001
		TESCondition					kConditions;		// 004
		TESObjectREFR*					pkTarget;			// 00C
		BSSimpleArray<ParentSpaceNode*>	kParentSpaceNodes;	// 010 - The four fields coud be a struct
		BSSimpleArray<TeleportLink*>	kTeleportLinks;		// 020
		UInt32							unk030;				// 030
		UInt32							unk034;				// 034
	};

	UInt32					uiObjectiveId;	// 004 Objective Index in the GECK
	BSStringT				kDisplayText;	// 008
	TESQuest*				pkQuest;		// 010
	BSSimpleList<Target*> 	kTargets;		// 014
	UInt32					unk01C;			// 01C
	UInt32					uiStatus;		// 020	bit0 = displayed, bit 1 = completed. 1 and 3 significant. If setting it to 3, quest flags bit1 will be set also.
};
static_assert(sizeof(BGSQuestObjective) == 0x24);
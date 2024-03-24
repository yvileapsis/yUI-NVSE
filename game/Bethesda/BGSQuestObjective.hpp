#pragma once

#include "BSString.hpp"
#include "BSSimpleList.hpp"
#include "BSSimpleArray.hpp"
#include "TESCondition.hpp"

class TESObjectREFR;
class TESQuest;

class BGSQuestObjective {
public:
	BGSQuestObjective();
	~BGSQuestObjective();

	enum {
		eQObjStatus_displayed = 1,
		eQObjStatus_completed = 2,
	};

	struct TargetData
	{
		TESObjectREFR* target;
		UInt8			flags;
		UInt8			filler[3];
	};

	struct ParentSpaceNode;
	struct TeleportLink
	{
		TESObjectREFR* door;
		UInt32			unk004[3];
	};	// 016

	struct Target
	{
		UInt8							byt000;				// 000
		UInt8							fill[3];			// 001
		TESCondition				conditions;			// 004
		TESObjectREFR* target;				// 00C
		BSSimpleArray<ParentSpaceNode*>	parentSpaceNodes;	// 010 - The four fields coud be a struct
		BSSimpleArray<TeleportLink*>		teleportLinks;		// 020
		UInt32							unk030;				// 030
		UInt32							unk034;				// 034
	};

	typedef BSSimpleList<BGSQuestObjective::Target> TargetList;

	UInt32			objectiveId;	// 004 Objective Index in the GECK
	BSStringT			displayText;	// 008
	TESQuest* quest;			// 010
	TargetList	targets;		// 014
	UInt32			unk01C;			// 01C
	UInt32			status;			// 020	bit0 = displayed, bit 1 = completed. 1 and 3 significant. If setting it to 3, quest flags bit1 will be set also.

	virtual void* Destroy(bool noDealloc);
};
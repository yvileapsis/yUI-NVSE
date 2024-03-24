#pragma once

#include "TESForm.hpp"
#include "TESScriptableForm.hpp"
#include "TESIcon.hpp"
#include "TESCondition.hpp"

class BGSQuestObjective;
struct ScriptEventList;

struct VariableInfo;

class TESQuest : public TESForm {
public:
	TESQuest();
	~TESQuest();

	virtual char* GetEditorName() const;			// 4E

	// bases
	TESScriptableForm			scriptable;			// 018
	TESIcon						icon;				// 024
	TESFullName					fullName;			// 030

	struct StageInfo {
		UInt8			stage;	// 00 stageID
		UInt8			unk001;	// 01 status ?
		UInt8			pad[2];	// 02
		BSSimpleList<void*>	unk004;	// 04 log entries
	};	// 00C

	union LocalVariableOrObjectivePtr
	{
		BGSQuestObjective* objective;
		VariableInfo* varInfoIndex;
	};

	UInt8						flags;						// 03C	bit0 is startGameEnabled/isRunning
	UInt8						priority;					// 03D
	UInt8						pad03E[2];					// 03E
	float						questDelayTime;				// 040
	BSSimpleList<StageInfo*>			stages;						// 044
	BSSimpleList<LocalVariableOrObjectivePtr*>	lVarOrObjectives;	// 04C	So: this list would contain both Objectives and LocalVariables !
	// That seems very strange but still, looking at Get/SetObjective... and ShowQuestVars there's no doubt.

	TESCondition			conditions;					// 054
	ScriptEventList* scriptEventList;			// 05C
	UInt8						currentStage;				// 060
	UInt8						pad061[3];					// 061
	BSStringT						editorName;					// 064
};

ASSERT_OFFSET(TESQuest, flags, 0x03C);
ASSERT_OFFSET(TESQuest, lVarOrObjectives, 0x04C);
ASSERT_OFFSET(TESQuest, conditions, 0x54);
ASSERT_SIZE(TESQuest, 0x6C);
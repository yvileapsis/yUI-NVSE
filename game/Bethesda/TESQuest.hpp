#pragma once
#include "TESForm.hpp"
#include "TESScriptableForm.hpp"
#include "TESIcon.hpp"
#include "TESCondition.hpp"

class BGSQuestObjective;

struct VariableInfo;

// 0x6C
class TESQuest :
	public TESForm,
	public TESScriptableForm,
	public TESIcon,
	public TESFullName
{
public:
	TESQuest();
	~TESQuest();

	virtual char* GetEditorName() const;			// 4E

	struct StageInfo {
		UInt8					stage;	// 00 stageID
		UInt8					unk001;	// 01 status ?
		UInt8					pad[2];	// 02
		BSSimpleList<void*>		unk004;	// 04 log entries
	};	// 00C

	union LocalVariableOrObjectivePtr
	{
		BGSQuestObjective*		objective;
		VariableInfo*			varInfoIndex;
	};

	UInt8						flags;					// 03C	bit0 is startGameEnabled/isRunning
	UInt8						priority;				// 03D
	UInt8						pad03E[2];				// 03E
	float						questDelayTime;			// 040
	BSSimpleList<StageInfo*>	stages;					// 044
	BSSimpleList<LocalVariableOrObjectivePtr*>	lVarOrObjectives;	// 04C	So: this list would contain both Objectives and LocalVariables !
	// That seems very strange but still, looking at Get/SetObjective... and ShowQuestVars there's no doubt.

	TESCondition			conditions;					// 054
	ScriptEventList*		scriptEventList;			// 05C
	UInt8					currentStage;				// 060
	UInt8					pad061[3];					// 061
	BSStringT<char>			editorName;					// 064

	BGSQuestObjective* GetObjective(UInt32 objectiveID) const;
};
static_assert(sizeof(TESQuest) == 0x6C);
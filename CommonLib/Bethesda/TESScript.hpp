#pragma once
#include "TESForm.hpp"
#include "VariableInfo.hpp"

struct ParamInfo
{
	const char* typeStr;
	UInt32		typeID;		// ParamType
	UInt32		isOptional;	// do other bits do things?
};

struct ScriptBuffer;
struct ScriptOperator;

class TESQuest;

class Script : public TESForm
{
public:
	Script() { ThisCall<Script*>(0x005AA0F0, this); };
	virtual ~Script() { ThisCall(0x005AA1A0, this); };

	// members

	struct RefVariable
	{
		String name;   // 000 variable name/editorID (not used at run-time)
		TESForm* form; // 008
		UInt32 varIdx; // 00C

		void Resolve(ScriptEventList* eventList);

		Script* GetReferencedScript() const;
	};

	struct RefList : BSSimpleList<RefVariable*>
	{
		UInt32 GetIndex(Script::RefVariable* refVar);
	};

	enum VariableType : UInt8
	{
		eVarType_Float = 0, // ref is also zero
		eVarType_Integer,

		// NVSE, return values only
		eVarType_String,
		eVarType_Array,
		eVarType_Ref,

		eVarType_Invalid
	};

	struct VarInfoList : BSSimpleList<VariableInfo*>
	{
		VariableInfo* GetVariableByName(const char* name);
	};
	// TODO: ask wall
	//	typedef Visitor<VarInfoList, VariableInfo> VarListVisitor;

	enum
	{
		eType_Object = 0,
		eType_Quest = 1,
		eType_Magic = 0x100,
		eType_Unk = 0x10000,
	};

	// 14
	struct ScriptInfo
	{
		UInt32 unusedVariableCount; // 00 (18)
		UInt32 numRefs;				// 04 (1C)
		UInt32 dataLength;			// 08 (20)
		UInt32 varCount;			// 0C (24)
		UInt16 type;				// 10 (28)
		UInt8 unk13;				// 13 (2B)
	};


	ScriptInfo info; // 018 / 018 / 02C
	char* text;		 // 02C / 02C / 040
	UInt8* m_val;	 // 030 / 030 / 044
	float unk34;				 // 034
	float questDelayTimeCounter; // 038	  - init'd to fQuestDelayTime, decremented by frametime each frame
	float secondsPassed;		 // 03C	  - only if you've modified fQuestDelayTime
	TESQuest* quest;			 // 040
	RefList refList;	 // 044 / 034 / 048 - ref variables and immediates
	VarInfoList varList; // 04C / 03C / 050 - local variable list

	RefVariable* GetRefFromRefList(UInt32 refIdx);
	VariableInfo* GetVariableInfo(UInt32 idx);

	UInt32 AddVariable(TESForm* form);
	void CleanupVariables();

	UInt32 Type() const { return info.type; }
	bool IsObjectScript() const { return info.type == eType_Object; }
	bool IsQuestScript() const { return info.type == eType_Quest; }
	bool IsMagicScript() const { return info.type == eType_Magic; }
	bool IsUnkScript() const { return info.type == eType_Unk; }

	VariableInfo* GetVariableByName(const char* varName);
	VariableInfo::EnumType GetVariableType(VariableInfo* var);

	bool IsUserDefinedFunction() const;

	static bool RunScriptLine(const char* text, TESObjectREFR* object = NULL);
	static bool RunScriptLine2(const char* text, TESObjectREFR* object = NULL, bool bSuppressOutput = true);

	static bool ParseParameters(ParamInfo* apParamInfo, void* apScriptData, UInt32 auiOpcodeOffsetPtr, TESObjectREFR* apThisObj, TESObjectREFR* apContainingObj, Script* apScriptObj, void* apEventList, char a8, ...);


	bool Execute(TESObjectREFR* thisObj, ScriptEventList* eventList, TESObjectREFR* containingObj, bool arg3)
	{ return ThisCall<bool>(0x005AC1E0, this, thisObj, eventList, containingObj, arg3); }

	void SetText(const char* text) { ThisCall(0x005ABE50, this, text); }
	bool Run(void* scriptContext, bool unkAlwaysOne, TESObjectREFR* object)
	{ return ThisCall<bool>(0x005AC400, this, scriptContext, unkAlwaysOne, object); }
	//static const UInt32 kScript_ExecuteFnAddr = 0x005AC1E0;

	ScriptEventList* CreateEventList();
	UInt32 GetVarCount() const;
	UInt32 GetRefCount() const;

	BSSimpleList<VariableInfo*>* GetVars();
	BSSimpleList<RefVariable*>* GetRefList();

	void Delete();
	bool Compile(ScriptBuffer* buffer);
};
static_assert(sizeof(Script) == 0x54);

using TESScript = Script;
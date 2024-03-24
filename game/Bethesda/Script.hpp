#pragma once

#include "TESForm.hpp"
#include "VariableInfo.hpp"
#include <CommandTable.hpp>

#if RUNTIME
#define SCRIPT_SIZE 0x54
static const UInt32 kScript_ExecuteFnAddr = 0x005AC1E0;
#elif EDITOR
#define SCRIPT_SIZE 0x48
static const UInt32 kScript_SetTextFnAddr = 0x005C27B0;
#endif

struct ScriptEventList;
struct ScriptBuffer;
struct ScriptOperator;

class TESQuest;

class Script : public TESForm
{
public:
	Script();
	virtual ~Script();

	// members

	class RefVariable : public BSMemObject {
	public:
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

	// 14
	struct ScriptInfo
	{
		UInt32 unusedVariableCount; // 00 (18)
		UInt32 numRefs;				// 04 (1C)
		UInt32 dataLength;			// 08 (20)
		UInt32 varCount;			// 0C (24)
		UInt16 type;				// 10 (28)
		bool compiled;				// 12 (2A)
		UInt8 unk13;				// 13 (2B)
	};

	enum
	{
		eType_Object = 0,
		eType_Quest = 1,
		eType_Magic = 0x100,
		eType_Unk = 0x10000,
	};
#if !RUNTIME
	UInt32 unk028; //     /     / 028
#endif
	ScriptInfo info; // 018 / 018 / 02C
	char* text;		 // 02C / 02C / 040
	UInt8* data;	 // 030 / 030 / 044
#if RUNTIME
	float unk34;				 // 034
	float questDelayTimeCounter; // 038      - init'd to fQuestDelayTime, decremented by frametime each frame
	float secondsPassed;		 // 03C      - only if you've modified fQuestDelayTime
	TESQuest* quest;			 // 040
#endif
	RefList refList;	 // 044 / 034 / 048 - ref variables and immediates
	VarInfoList varList; // 04C / 03C / 050 - local variable list
#if !RUNTIME
	void* unk050; //     /     / 050
	UInt8 unk054; //	   /     / 054
	UInt8 pad055[3];
#endif

	RefVariable* GetRefFromRefList(UInt32 refIdx);
	VariableInfo* GetVariableInfo(UInt32 idx);

	UInt32 AddVariable(TESForm* form);
	void CleanupVariables(void);

	UInt32 Type() const { return info.type; }
	bool IsObjectScript() const { return info.type == eType_Object; }
	bool IsQuestScript() const { return info.type == eType_Quest; }
	bool IsMagicScript() const { return info.type == eType_Magic; }
	bool IsUnkScript() const { return info.type == eType_Unk; }

	VariableInfo* GetVariableByName(const char* varName);
	Script::VariableType GetVariableType(VariableInfo* var);

	bool IsUserDefinedFunction() const;

	static bool RunScriptLine(const char* text, TESObjectREFR* object = NULL);
	static bool RunScriptLine2(const char* text, TESObjectREFR* object = NULL, bool bSuppressOutput = true);

	static bool ParseParameters(ParamInfo* apParamInfo, void* apScriptData, UInt32 auiOpcodeOffsetPtr, TESObjectREFR* apThisObj, TESObjectREFR* apContainingObj, Script* apScriptObj, void* apEventList, char a8, ...);

	// no changed flags (TESForm flags)
	MEMBER_FN_PREFIX(Script);
#if RUNTIME
	// arg3 appears to be true for result scripts (runs script even if dataLength <= 4)
	DEFINE_MEMBER_FN(Execute, bool, kScript_ExecuteFnAddr, TESObjectREFR* thisObj, ScriptEventList* eventList, TESObjectREFR* containingObj, bool arg3);
	DEFINE_MEMBER_FN(Constructor, Script*, 0x005AA0F0);
	DEFINE_MEMBER_FN(SetText, void, 0x005ABE50, const char* text);
	DEFINE_MEMBER_FN(Run, bool, 0x005AC400, void* scriptContext, bool unkAlwaysOne, TESObjectREFR* object);
	DEFINE_MEMBER_FN(Destructor, void, 0x005AA1A0);
#endif
	ScriptEventList* CreateEventList();
	UInt32 GetVarCount() const;
	UInt32 GetRefCount() const;

	BSSimpleList<VariableInfo*>* GetVars();
	BSSimpleList<RefVariable*>* GetRefList();

	void Delete();

	static game_unique_ptr<Script> MakeUnique();

	bool Compile(ScriptBuffer* buffer);
};

ASSERT_SIZE(Script, 0x54u);
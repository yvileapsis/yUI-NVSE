#pragma once
#include "Utilities.hpp"
#include "TESForm.h"

struct ScriptEventList;
struct ScriptBuffer;

extern CRITICAL_SECTION	csGameScript;				// trying to avoid what looks like concurrency issues

// 54 / 48
class Script : public TESForm
{
public:
	Script();
	~Script();

	// members

	struct RefVariable
	{
		String	name;		// 000 variable name/editorID (not used at run-time)
		TESForm	* form;		// 008
		UInt32	varIdx;		// 00C

		void	Resolve(ScriptEventList * eventList);
	};

	struct RefListEntry
	{
		RefVariable		* var;
		RefListEntry	* next;

		RefVariable* Info() const { return var; }
		RefListEntry* Next() const { return next; }
		void SetNext(RefListEntry* nextEntry) { next = nextEntry; }
		RefVariable* GetRefVariableByName(const char* name);
		UInt32 GetIndex(RefVariable* refVar);
	};
	typedef Visitor<RefListEntry, RefVariable> RefListVisitor;

	enum {
		eVarType_Float = 0,			//ref is also zero
		eVarType_Integer,

		// NVSE, return values only
		eVarType_String,
		eVarType_Array,
		eVarType_Ref,

		eVarType_Invalid
	};

	struct VarInfoEntry
	{
		VariableInfo	* data;
		VarInfoEntry	* next;

		VariableInfo* Info() const { return data; }
		VarInfoEntry* Next() const { return next; }

		VariableInfo* GetVariableByName(const char* name);
	};
	typedef Visitor<VarInfoEntry, VariableInfo> VarListVisitor;

	// 14
	struct ScriptInfo
	{
		UInt32	unk0;		// 00 (18)
		UInt32	numRefs;	// 04 (1C)
		UInt32	dataLength;	// 08 (20)
		UInt32	varCount;	// 0C (24)
		UInt16	type;		// 10 (28)
		bool	compiled;	// 12 (2A)
		UInt8   unk13;      // 13 (2B)
	};

	enum {
		eType_Object = 0,
		eType_Quest = 1,
		eType_Magic = 0x100,
		eType_Unk = 0x10000,
	};
	ScriptInfo		info;					// 018 / 018 / 02C
	char			* text;					// 02C / 02C / 040
	void			* data;					// 030 / 030 / 044
	float			unk34;					// 034
	float			questDelayTimeCounter;	// 038      - init'd to fQuestDelayTime, decremented by frametime each frame
	float			secondsPassed;			// 03C      - only if you've modified fQuestDelayTime
	TESQuest*		quest;					// 040
	RefListEntry	refList;				// 044 / 034 / 048 - ref variables and immediates
	VarInfoEntry	varList;				// 04C / 03C / 050 - local variable list

	RefVariable *	GetRefFromRefList(UInt32 refIdx);
	VariableInfo*	GetVariableInfo(UInt32 idx);

	UInt32			AddVariable(TESForm * form);
	void			CleanupVariables();

	UInt32			Type() const { return info.type; }
	bool			IsObjectScript() const {return info.type == eType_Object; }
	bool			IsQuestScript() const { return info.type == eType_Quest; }
	bool			IsMagicScript() const { return info.type == eType_Magic; }
	bool			IsUnkScript() const { return info.type == eType_Unk; }

	VariableInfo*	GetVariableByName(const char* varName);
	UInt32			GetVariableType(VariableInfo* var);

	bool			IsUserDefinedFunction() const;

	bool Execute(TESObjectREFR* thisObj, ScriptEventList* eventList, TESObjectREFR* containingObj, bool arg3)
	{ return ThisCall<bool>(0x005AC1E0, this, thisObj, eventList, containingObj, arg3); }

	Script* Constructor() { return ThisCall<Script*>(0x005AA0F0, this); }
	void Destructor() { ThisCall(0x005AA1A0, this); }
	void SetText(const char* text) { ThisCall(0x005ABE50, this, text); }
	bool Run(void* scriptContext, bool unkAlwaysOne, TESObjectREFR* object)
	{ return ThisCall<bool>(0x005AC400, this, scriptContext, unkAlwaysOne, object); }

	ScriptEventList	* CreateEventList();
	UInt32 GetVarCount() const;
	UInt32 GetRefCount() const;

	TList<VariableInfo>* GetVars();
	TList<RefVariable>* GetRefList();
	static Script* CreateScript();
	void DeleteScript() const;

	static Script* CompileFromText(const std::string& text, const std::string& name);
};
static_assert(sizeof(Script) == 0x54);

struct ConditionEntry
{
	struct Data
	{
		union Param
		{
			float	number;
			TESForm	* form;
		};

		// ### TODO: this
		UInt32		operatorAndFlags;	// 00
		float		comparisonValue;	// 04
		UInt16		functionIndex;		// 08 is opcode & 0x0FFF
		UInt16		unk0A;
		Param		param1;				// 0C
		Param		param2;				// 10
		UInt32		unk14;
	};

	Data			* data;
	ConditionEntry	* next;
};

// 6C
struct QuestStageItem
{
	UInt32			unk00;			// 00
	ConditionEntry	conditionList;	// 04
	Script			resultScript;	// 0C
	UInt32			unk5C;			// 5C disk offset to log text records? consistent within a single quest
	UInt8			index;			// 60 sequential
	bool			hasLogText;		// 61
	UInt8			unk62[2];		// 62 pad?
	UInt32			logDate;		// 64
	TESQuest		* owningQuest;	// 68;
};

static_assert(sizeof(QuestStageItem) == (0x54 + 0x1C));

// 41C
struct ScriptLineBuffer
{
	static const UInt32	kBufferSize = 0x200;

	UInt32				lineNumber;			// 000 counts blank lines too
	char				paramText[0x200];	// 004 portion of line text following command
	UInt32				paramTextLen;		// 204
	UInt32				lineOffset;			// 208
	UInt8				dataBuf[0x200];		// 20C
	UInt32				dataOffset;			// 40C
	UInt32				cmdOpcode;			// 410 not initialized. Opcode of command being parsed
	UInt32				callingRefIndex;	// 414 not initialized. Zero if cmd not invoked with dot syntax
	UInt32				errorCode;			// 418

	// these write data and update dataOffset
	bool Write(const void* buf, UInt32 bufsize);
	bool WriteFloat(double buf);
	bool WriteString(const char* buf);
	bool Write32(UInt32 buf);
	bool Write16(UInt16 buf);
	bool WriteByte(UInt8 buf);
};

// size 0x58? Nothing initialized beyond 0x50.
struct ScriptBuffer
{	
	template <typename tData> struct Node
	{
		tData		* data;
		Node<tData>	* next;
	};

	enum RuntimeMode
	{
		kEditor = 0,
		kGameConsole = 1,
	};

	ScriptBuffer();
	~ScriptBuffer();

	const char* scriptText;		// 000
	UInt32			textOffset;			// 004 
	RuntimeMode		runtimeMode;		// 008
	String			scriptName;			// 00C
	UInt32			errorCode;			// 014
	bool			partialScript;		// 018
	UInt8			pad019[3];			// 019
	UInt32			curLineNumber;		// 01C
	UInt8			* scriptData;		// 020 pointer to 0x4000-byte array
	UInt32			dataOffset;			// 024
	UInt32			unk028;				// 028
	UInt32			numRefs;			// 02C
	UInt32			unk030;				// 030
	UInt32			varCount;			// 034 script->varCount
	UInt8			scriptType;			// 038 script->type
	UInt8			unk039;				// 039 script->unk35
	UInt8			unk03A[2];
	Script::VarInfoEntry	vars;		// 03C
	Script::RefListEntry	refVars;	// 044 probably ref vars
	Script*			currentScript;				// 04C num lines?
	Node<ScriptLineBuffer>	lines;		// 050
	// nothing else initialized

	// convert a variable or form to a RefVar, add to refList if necessary
	Script::RefVariable* ResolveRef(const char* refName);
	UInt32	GetRefIdx(Script::RefVariable* ref);
	UInt32	GetVariableType(VariableInfo* varInfo, Script::RefVariable* refVar);
};


// represents the currently executing script context
class ScriptRunner
{
public:
	static const UInt32	kStackDepth = 10;

	enum
	{
		kStackFlags_IF = 1 << 0,
		kStackFlags_ELSEIF = 1 << 1,
		/* ELSE and ENDIF modify the above flags*/
	};

	TESObjectREFR*		containingObj;		/*00*/  // set when executing scripts on inventory objects
	TESForm*			callingRefBaseForm;	/*04*/ 
	ScriptEventList*	eventList;			/*08*/ 
	UInt32				unk0C;				/*0C*/ 
	UInt32				unk10;				/*10*/  // pointer? set to NULL before executing an instruction
	Script*				script;				/*14*/ 
	UInt32				unk18;				/*18*/ // set to 6 after a failed expression evaluation
	UInt32				unk1C;				/*1C*/ // set to Expression::errorCode
	UInt32				ifStackDepth;		/*20*/ 
	UInt32				ifStack[kStackDepth];		/*24*/ // stores flags
	UInt32				unk4C[(0xA0 - 0x4C) >> 2];	/*4C*/ 
	UInt8				invalidReferences;	/*A0*/ // set when the dot operator fails to resolve a reference (inside the error message handler)
	UInt8				unkA1;				/*A1*/// set when the executing CommandInfo's 2nd flag bit (+0x25) is set
	UInt16				padA2;				/*A2*/ 
};
static_assert(sizeof(ScriptRunner) == 0xA4);

// unk1 = 0
// unk2 = 0
// callback = may be NULL apparently
// unk4 = 0
// unk5 = 0x17 (why?)
// unk6 = 0
// unk7 = 0
// then buttons
// then NULL

void ShowCompilerError(ScriptLineBuffer* lineBuf, const char* fmt, ...);

// only records individual objects if there's a block that matches it
// ### how can it tell?
struct ScriptEventList
{
	enum
	{
		kEvent_OnAdd					= 1 << 0,
		kEvent_OnEquip					= 1 << 1,		// Called on Item and on Refr
		kEvent_OnActorEquip				= kEvent_OnEquip,	// presumably the game checks the type of the object
		kEvent_OnDrop					= 1 << 2,
		kEvent_OnUnequip				= 1 << 3,
		kEvent_OnActorUnequip			= kEvent_OnUnequip,
		kEvent_OnDeath					= 1 << 4,
		kEvent_OnMurder					= 1 << 5,
		kEvent_OnCombatEnd				= 1 << 6,		// See 0x008A083C
		kEvent_OnHit					= 1 << 7,		// See 0x0089AB12
		kEvent_OnHitWith				= 1 << 8,		// TESObjectWEAP*	0x0089AB2F
		kEvent_OnPackageStart			= 1 << 9,
		kEvent_OnPackageDone			= 1 << 10,
		kEvent_OnPackageChange			= 1 << 11,
		kEvent_OnLoad					= 1 << 12,
		kEvent_OnMagicEffectHit			= 1 << 13,		// EffectSetting* 0x0082326F
		kEvent_OnSell					= 1 << 14,		// 0x0072FE29 and 0x0072FF05, linked to 'Barter Amount Traded' Misc Stat
		kEvent_OnStartCombat			= 1 << 15,

		kEvent_OnOpen					= 1 << 16,		// while opening some container, not all
		kEvent_OnClose					= 1 << 17,		// idem
		kEvent_SayToDone				= 1 << 18,		// in Func0050 0x005791C1 in relation to SayToTopicInfo (OnSayToDone? or OnSayStart/OnSayEnd?)
		kEvent_OnGrab					= 1 << 19,		// 0x0095FACD and 0x009604B0 (same func which is called from PlayerCharacter_func001B and 0021)
		kEvent_OnRelease				= 1 << 20,		// 0x0047ACCA in relation to container
		kEvent_OnDestructionStageChange	= 1 << 21,		// 0x004763E7/0x0047ADEE
		kEvent_OnFire					= 1 << 22,		// 0x008BAFB9 (references to package use item and use weapon are close)

		kEvent_OnTrigger				= 1 << 28,		// 0x005D8D6A	Cmd_EnterTrigger_Execute
		kEvent_OnTriggerEnter			= 1 << 29,		// 0x005D8D50	Cmd_EnterTrigger_Execute
		kEvent_OnTriggerLeave			= 1 << 30,		// 0x0062C946	OnTriggerLeave ?
		kEvent_OnReset					= 1 << 31		// 0x0054E5FB
	};

	struct Event
	{
		TESForm	* object;
		UInt32	eventMask;
	};

	struct VarEntry;

	struct Var
	{
		UInt32		id;
		VarEntry	* nextEntry;
		double		data;

		UInt32 GetFormId();
	};

	struct VarEntry
	{
		Var			* var;
		VarEntry	* next;
	};

	struct Struct010
	{
		UInt8 unk00[8];
	};

	typedef TList<Event> EventList;

	Script			* m_script;		// 00
	UInt32			  m_unk1;			// 04
	EventList		* m_eventList;	// 08
	VarEntry		* m_vars;		// 0C
	Struct010		* unk010;		// 10

	Var *	GetVariable(UInt32 id);
	UInt32	ResetAllVariables();

	void	Destructor();
	TList<Var>* GetVars() const;
	void					EventCreate(UInt32 eventCode, TESObjectREFR* container);

};

ScriptEventList* EventListFromForm(TESForm* form);

typedef bool (* _MarkBaseExtraListScriptEvent)(TESForm* target, BaseExtraList* extraList, UInt32 eventMask);
extern const _MarkBaseExtraListScriptEvent MarkBaseExtraListScriptEvent;

typedef void (_cdecl * _DoCheckScriptRunnerAndRun)(TESObjectREFR* refr, BaseExtraList* extraList);
extern const _DoCheckScriptRunnerAndRun DoCheckScriptRunnerAndRun;

struct ExtractedParam
{
	// float/double types are kept as pointers
	// this avoids problems with storing invalid floats/doubles in to the fp registers which has a side effect
	// of corrupting data

	enum
	{
		kType_Unknown = 0,
		kType_String,		// str
		kType_Imm32,		// imm
		kType_Imm16,		// imm
		kType_Imm8,			// imm
		kType_ImmDouble,	// immDouble
		kType_Form,			// form
	};

	UInt8	type;
	bool	isVar;	// if true, data is stored in var, otherwise it's immediate

	union
	{
		// immediate
		UInt32			imm;
		const double	* immDouble;
		TESForm			* form;
		struct
		{
			const char	* buf;
			UInt32		len;
		} str;

		// variable
		struct 
		{
			ScriptEventList::Var	* var;
			ScriptEventList			* parent;
		} var;
	} data;
};


UInt32 GetDeclaredVariableType(const char* varName, const char* scriptText);	// parses scriptText to determine var type
Script* GetScriptFromForm(TESForm* form);
CommandInfo* GetEventCommandInfo(UInt16 opcode);

// Gets the real script data ptr, as it can be a pointer to a buffer on the stack in case of vanilla expressions in set and if statements
UInt8* GetScriptDataPosition(Script* script, void* scriptDataIn, const UInt32* opcodeOffsetPtrIn);


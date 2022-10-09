#pragma once
#include <GameTypes.h>
#include <NiNodes.h>
#include <GameScript.h>

constexpr auto playerID = 0x7;
constexpr auto playerRefID = 0x14;

__forceinline bool MenuMode() { return CdeclCall<bool>(0x702360); }

extern bool extraTraces;
extern bool alternateUpdate3D;
extern bool s_InsideOnActorEquipHook;
extern UInt32 s_CheckInsideOnActorEquipHook;


typedef TESForm * (* _CreateFormInstance)(UInt8 type);
extern const _CreateFormInstance CreateFormInstance;


const char* GetActorValueString(UInt32 actorValue); // should work now
UInt32 GetActorValueForString(const char* strActorVal, bool bForScript = false);

typedef char * (* _GetActorValueName)(UInt32 actorValueCode);
extern const _GetActorValueName GetActorValueName;
UInt32 GetActorValueMax(UInt32 actorValueCode);


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

struct NVSEStringVarInterface;
	// Problem: plugins may want to use %z specifier in format strings, but don't have access to StringVarMap
	// Could change params to ExtractFormatStringArgs to include an NVSEStringVarInterface* but
	//  this would break existing plugins
	// Instead allow plugins to register their NVSEStringVarInterface for use
	// I'm sure there is a better way to do this but I haven't found it

// only records individual objects if there's a block that matches it
// ### how can it tell?
struct ScriptEventList
{
	enum
	{
		kEvent_OnAdd			= 0x00000001,
		kEvent_OnEquip			= 0x00000002,		// Called on Item and on Refr
		kEvent_OnActorEquip		= kEvent_OnEquip,	// presumably the game checks the type of the object
		kEvent_OnDrop			= 0x00000004,
		kEvent_OnUnequip		= 0x00000008,
		kEvent_OnActorUnequip	= kEvent_OnUnequip,
		kEvent_OnDeath			= 0x00000010,
		kEvent_OnMurder			= 0x00000020,
		kEvent_OnCombatEnd		= 0x00000040,		// See 0x008A083C
		kEvent_OnHit			= 0x00000080,		// See 0x0089AB12
		kEvent_OnHitWith		= 0x00000100,		// TESObjectWEAP*	0x0089AB2F
		kEvent_OnPackageStart	= 0x00000200,
		kEvent_OnPackageDone	= 0x00000400,
		kEvent_OnPackageChange	= 0x00000800,
		kEvent_OnLoad			= 0x00001000,
		kEvent_OnMagicEffectHit = 0x00002000,		// EffectSetting* 0x0082326F
		kEvent_OnSell			= 0x00004000,		// 0x0072FE29 and 0x0072FF05, linked to 'Barter Amount Traded' Misc Stat
		kEvent_OnStartCombat	= 0x00008000,

		kEvent_OnOpen			= 0x00010000,		// while opening some container, not all
		kEvent_OnClose			= 0x00020000,		// idem
		kEvent_SayToDone		= 0x00040000,		// in Func0050 0x005791C1 in relation to SayToTopicInfo (OnSayToDone? or OnSayStart/OnSayEnd?)
		kEvent_OnGrab			= 0x00080000,		// 0x0095FACD and 0x009604B0 (same func which is called from PlayerCharacter_func001B and 0021)
		kEvent_OnRelease		= 0x00100000,		// 0x0047ACCA in relation to container
		kEvent_OnDestructionStageChange	= 0x00200000,		// 0x004763E7/0x0047ADEE
		kEvent_OnFire			= 0x00400000,		// 0x008BAFB9 (references to package use item and use weapon are close)

		kEvent_OnTrigger		= 0x10000000,		// 0x005D8D6A	Cmd_EnterTrigger_Execute
		kEvent_OnTriggerEnter	= 0x20000000,		// 0x005D8D50	Cmd_EnterTrigger_Execute
		kEvent_OnTriggerLeave	= 0x40000000,		// 0x0062C946	OnTriggerLeave ?
		kEvent_OnReset			= 0x80000000		// 0x0054E5FB
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

	void	Dump(void);
	Var *	GetVariable(UInt32 id);
	UInt32	ResetAllVariables();

	void	Destructor();
	TList<Var>* GetVars() const;
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

enum EActorVals {
	eActorVal_Aggression			= 0,
	eActorVal_Confidence			= 1,
	eActorVal_Energy				= 2,
	eActorVal_Responsibility		= 3,
	eActorVal_Mood					= 4,

	eActorVal_Strength				= 5,
	eActorVal_Perception			= 6,
	eActorVal_Endurance				= 7,
	eActorVal_Charisma				= 8,
	eActorVal_Intelligence			= 9,
	eActorVal_Agility				= 10,
	eActorVal_Luck					= 11,
	eActorVal_SpecialStart = eActorVal_Strength,
	eActorVal_SpecialEnd = eActorVal_Luck,

	eActorVal_ActionPoints			= 12,
	eActorVal_CarryWeight			= 13,
	eActorVal_CritChance			= 14,
	eActorVal_HealRate				= 15,
	eActorVal_Health				= 16,
	eActorVal_MeleeDamage			= 17,
	eActorVal_DamageResistance		= 18,
	eActorVal_PoisonResistance		= 19,
	eActorVal_RadResistance			= 20,
	eActorVal_SpeedMultiplier		= 21,
	eActorVal_Fatigue				= 22,
	eActorVal_Karma					= 23,
	eActorVal_XP					= 24,

	eActorVal_Head					= 25,
	eActorVal_Torso					= 26,
	eActorVal_LeftArm				= 27,
	eActorVal_RightArm				= 28,
	eActorVal_LeftLeg				= 29,
	eActorVal_RightLeg				= 30,
	eActorVal_Brain					= 31,
	eActorVal_BodyPartStart = eActorVal_Head,
	eActorVal_BodyPartEnd = eActorVal_Brain,

	eActorVal_Barter				= 32,
	eActorVal_BigGuns				= 33,
	eActorVal_EnergyWeapons			= 34,
	eActorVal_Explosives			= 35,
	eActorVal_Lockpick				= 36,
	eActorVal_Medicine				= 37,
	eActorVal_MeleeWeapons			= 38,
	eActorVal_Repair				= 39,
	eActorVal_Science				= 40,
	eActorVal_Guns					= 41,
	eActorVal_Sneak					= 42,
	eActorVal_Speech				= 43,
	eActorVal_Survival				= 44,
	eActorVal_Unarmed				= 45,
	eActorVal_SkillsStart = eActorVal_Barter,
	eActorVal_SkillsEnd = eActorVal_Unarmed,

	eActorVal_InventoryWeight		= 46,
	eActorVal_Paralysis				= 47,
	eActorVal_Invisibility			= 48,
	eActorVal_Chameleon				= 49,
	eActorVal_NightEye				= 50,
	eActorVal_Turbo					= 51,
	eActorVal_FireResistance		= 52,
	eActorVal_WaterBreathing		= 53,
	eActorVal_RadLevel				= 54,
	eActorVal_BloodyMess			= 55,
	eActorVal_UnarmedDamage			= 56,
	eActorVal_Assistance			= 57,

	eActorVal_ElectricResistance	= 58,
	eActorVal_FrostResistance		= 59,

	eActorVal_EnergyResistance		= 60,
	eActorVal_EMPResistance			= 61,
	eActorVal_Var1Medical			= 62,
	eActorVal_Var2					= 63,
	eActorVal_Var3					= 64,
	eActorVal_Var4					= 65,
	eActorVal_Var5					= 66,
	eActorVal_Var6					= 67,
	eActorVal_Var7					= 68,
	eActorVal_Var8					= 69,
	eActorVal_Var9					= 70,
	eActorVal_Var10					= 71,

	eActorVal_IgnoreCrippledLimbs	= 72,
	eActorVal_Dehydration			= 73,
	eActorVal_Hunger				= 74,
	eActorVal_Sleepdeprevation		= 75,
	eActorVal_Damagethreshold		= 76,
	eActorVal_FalloutMax = eActorVal_Damagethreshold,
	eActorVal_NoActorValue = 256,
};

// A plugin author requested the ability to use OBSE format specifiers to format strings with the args
// coming from a source other than script.
// So changed ExtractFormattedString to take an object derived from following class, containing the args
// Probably doesn't belong in GameAPI.h but utilizes a bunch of stuff defined here and can't think of a better place for it
class FormatStringArgs
{
public:
	enum argType {
		kArgType_Float,
		kArgType_Form		// TESForm*
	};

	virtual bool Arg(argType asType, void * outResult) = 0;	// retrieve next arg
	virtual bool SkipArgs(UInt32 numToSkip) = 0;			// skip specified # of args
	virtual bool HasMoreArgs() = 0;
	virtual char *GetFormatString() = 0;						// return format string
};
//
struct ToBeNamed
{
	char		m_path[0x104];	// 0000
	BSFile*		m_file;			// 0104
	UInt32		m_unk0108;		// 0108
	UInt32		m_offset;		// 010C
};

template <typename T_Key, typename T_Data>
class NiTMap : public NiTMapBase<T_Key, T_Data>
{
public:
	NiTMap();
	~NiTMap();
};

enum Coords
{
	kCoords_X = 0,	// 00
	kCoords_Y,		// 01
	kCoords_Z,		// 02
	kCoords_Max		// 03
};

struct NavMeshVertex
{
	float coords[kCoords_Max];	// 000
};	// 00C

enum Vertices
{
	kVertices_0 = 0,	// 00
	kVertices_1,		// 01
	kVertices_2,		// 02
	kVertices_Max		// 03
};

enum Sides
{
	kSides_0_1 = 0,	// 00
	kSides_1_2,		// 01
	kSides_2_0,		// 02
	kSides_Max		// 03
};

struct NavMeshTriangle
{
	SInt16	verticesIndex[kVertices_Max];	// 000
	SInt16	sides[kSides_Max];				// 006
	UInt32	flags;							// 00C
};	// Alloc'd by 0x10

struct NavMeshInfo;

struct EdgeExtraInfo
{
	struct Connection
	{
		NavMeshInfo*	navMeshInfo;
		SInt16			triangle;
	};

	UInt32	unk000;			// 00
	Connection connectTo;	// 04
};	// Alloc'd by 0x0C

struct NavMeshTriangleDoorPortal
{
	TESObjectREFR	* door;	// 00
	UInt16			unk004;	// 04
	UInt16			pad006;	// 06
};	// Alloc'd to 0x08

struct NavMeshCloseDoorInfo
{
	UInt32	unk000;	// 00
	UInt32	unk004;	// 04
};	// Alloc'd to 0x08

struct NavMeshPOVData;
struct ObstacleData;
struct ObstacleUndoData;

struct NavMeshStaticAvoidNode
{
	UInt32	unk000;	// 00
	UInt32	unk004;	// 04
	UInt32	unk008;	// 08
	UInt32	unk00C;	// 0C
	UInt32	unk010;	// 10
	UInt32	unk014;	// 14
	UInt32	unk018;	// 18
	UInt32	unk01C;	// 1C
	UInt32	unk020;	// 20
	UInt32	unk024;	// 24
};	// Alloc'd to 0x28


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

class ScrapHeapQueue
{
public:
	struct QueuedCmdCall
	{
		UInt32			opcode;		// 00
		void*			cmdAddr;	// 04
		UInt32			thisObj;	// 08	refID
		UInt32			numArgs;	// 0C
		FunctionArg		args[4];	// 10

		QueuedCmdCall(void* _cmdAddr, UInt32 _thisObj, UInt8 _numArgs) : opcode(0x2B), cmdAddr(_cmdAddr),
		                                                                 thisObj(_thisObj), numArgs(_numArgs), args{}
		{
		}
	};

	static ScrapHeapQueue*		GetSingleton() { return *reinterpret_cast<ScrapHeapQueue**>(0x11DF1A8); }
	__forceinline void			AddQueuedCmdCall(QueuedCmdCall qCall) { ThisCall(0x87D160, this, &qCall); }
};

// Gets the real script data ptr, as it can be a pointer to a buffer on the stack in case of vanilla expressions in set and if statements
UInt8* GetScriptDataPosition(Script* script, void* scriptDataIn, const UInt32* opcodeOffsetPtrIn);

struct Timer
{
	UInt8 disableCounter;		// 00
	UInt8 gap01[3];				// 01
	float fpsClamp;				// 04
	float fpsClampRemainder;	// 08
	float secondsPassed;		// 0C
	float lastSecondsPassed;	// 10
	UInt32 msPassed;			// 14
	UInt32 unk18;				// 18
	byte isChangeTimeMultSlowly;// 1C
	byte unk1D;					// 1D
	byte unk1E;					// 1E
	byte unk1F;					// 1F
};

struct TimeGlobal : Timer
{
	float unk20;  // 020
	float unk24;  // 024
	float unk28;  // 028

	static TimeGlobal*				GetSingleton() { return reinterpret_cast<TimeGlobal*>(0x11F6394); };
	__forceinline void				Set(const Float32 value, const char isImmediateChange) { ThisCall<void>(0xAA4DB0, this, value, isImmediateChange); }
	static __forceinline Float32	Get() { return *reinterpret_cast<Float32*>(0x11AC3A0); }
	static __forceinline Float32	GetTarget() { return *reinterpret_cast<Float32*>(0x11AC3A4); }
};

__forceinline NiTPointerMap<TESForm>* GetAllForms() { return *reinterpret_cast<NiTPointerMap<TESForm>**>(0x11C54C0); }
__forceinline TESForm* GetFormByID(const char* editorID) { return CdeclCall<TESForm*>(0x483A00, editorID); };
__forceinline TESForm* GetFormByID(UInt32 refID) { return GetAllForms()->Lookup(refID); };
TESForm* GetFormByID(const char* mod, UInt32 refID);
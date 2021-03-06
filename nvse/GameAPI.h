#pragma once
#include <CommandTable.h>
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

void PrintConsole(const char * fmt, ...);

typedef TESForm * (* _CreateFormInstance)(UInt8 type);
extern const _CreateFormInstance CreateFormInstance;

bool IsConsoleMode();
bool GetConsoleEcho();
void SetConsoleEcho(bool doEcho);
const char* GetActorValueString(UInt32 actorValue); // should work now
UInt32 GetActorValueForString(const char* strActorVal, bool bForScript = false);

typedef char * (* _GetActorValueName)(UInt32 actorValueCode);
extern const _GetActorValueName GetActorValueName;
UInt32 GetActorValueMax(UInt32 actorValueCode);

typedef void (* _ShowMessageBox_Callback)(void);
extern const _ShowMessageBox_Callback ShowMessageBox_Callback;

// unk1 = 0
// unk2 = 0
// callback = may be NULL apparently
// unk4 = 0
// unk5 = 0x17 (why?)
// unk6 = 0
// unk7 = 0
// then buttons
// then NULL
typedef bool (* _ShowMessageBox)(const char * message, UInt32 unk1, UInt32 unk2, _ShowMessageBox_Callback callback, UInt32 unk4, UInt32 unk5, float unk6, float unk7, ...);
extern const _ShowMessageBox ShowMessageBox;

// set to scriptObj->refID after calling ShowMessageBox()
// GetButtonPressed checks this before returning a value, if it doesn't match it returns -1
typedef UInt32 * _ShowMessageBox_pScriptRefID;
extern const _ShowMessageBox_pScriptRefID ShowMessageBox_pScriptRefID;
typedef UInt8 * _ShowMessageBox_button;
extern const _ShowMessageBox_button ShowMessageBox_button;

// can be passed to QueueUIMessage to determine Vaultboy icon displayed
enum eEmotion {
	happy	= 0,
	sad		= 1,
	neutral = 2,
	pain	= 3
};

typedef bool (* _QueueUIMessage)(const char* msg, UInt32 emotion, const char* ddsPath, const char* soundName, float msgTime, bool maybeNextToDisplay);
extern const _QueueUIMessage QueueUIMessage;

const UInt32 kMaxMessageLength = 0x4000;

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

class ChangesMap;
class InteriorCellNewReferencesMap;
class ExteriorCellNewReferencesMap;
class NumericIDBufferMap;

//
struct ToBeNamed
{
	char		m_path[0x104];	// 0000
	BSFile*		m_file;			// 0104
	UInt32		m_unk0108;		// 0108
	UInt32		m_offset;		// 010C
};

// Form type class: use to preload some information for created objects (?) refr and Cells
struct formTypeClassData
{
	typedef UInt8 EncodedID[3];	// Codes the refID on 3 bytes, as used in changed forms and save refID mapping

	struct Data01 // Applies to CELL where changeFlags bit30 (Detached CELL) and bit29 (CHANGE_CELL_EXTERIOR_CHAR) are set
	{
		UInt16	worldspaceIndex;	// 00 Index into visitedWorldspaces		goes into unk000
		UInt8	coordX;				// 02	goes into unk004
		UInt8	coordY;				// 03	goes into unk008, paired with 002
		UInt8	detachTime;			// 04	goes into unk00C
	};

	struct Data02 // Applies to CELL where changeFlags bit30 (Detached CELL) and bit 28 (CHANGE_CELL_EXTERIOR_SHORT) are set and changeFlags bit29 is clear
	{
		UInt16	worldspaceIndex;	// 00 Index into visitedWorldspaces		goes into unk000
		UInt16	coordX;				// 02	goes into unk004
		UInt16	coordY;				// 03	goes into unk008, paired with 002
		UInt32	detachTime;			// 04	goes into unk00C
	};

	// The difference between the two preceding case seems to be how big the data (coordinates?) are

	struct Data03 // Applies to CELL where changeFlags bit30 (Detached CELL) is set and changeFlags bit28 and bit29 are clear
	{
		UInt32	detachTime;	// 00	goes into unk00C. Null goes into unk000, 004 and 008
	};

	struct Data04 // Applies to references where changeFlags bit3 (CHANGE_REFR_CELL_CHANGED) is clear and
					// either bit1 (CHANGE_REFR_MOVE) or bit2 (CHANGE_REFR_HAVOK_MOVE) is set
	{
		EncodedID	cellOrWorldspaceID;	// 000	goes into unk000, Null goes into unk004, 008, 00C, 010 and byt02C
		float		posX;	// 003	goes into unk014
		float		posY;	// 007	goes into unk018, associated with unk003
		float		posZ;	// 00B	goes into unk01C, associated with unk003	(pos?)
		float		rotX;	// 00F	goes into unk020
		float		rotY;	// 013	goes into unk024, associated with unk00F
		float		rotZ;	// 017	goes into unk028, associated with unk00F	(rot?)
	};

	struct Data05 // Applies to created objects (ie 0xFFnnnnnn)
	{
		EncodedID	cellOrWorldspaceID;	// 000	goes into unk000
		float		posX;	// 003	goes into unk014
		float		posY;	// 007	goes into unk018, associated with unk003
		float		posZ;	// 00B	goes into unk01C, associated with unk003	(pos?)
		float		rotX;	// 00F	goes into unk020
		float		rotY;	// 013	goes into unk024, associated with unk024
		float		rotZ;	// 017	goes into unk028, associated with unk028	(rot?)
		UInt8		flags;	// 01B	goes into unk02C	bit0 always set, bit1 = ESP or persistent, bit2 = Byt081 true
		EncodedID	baseFormID;	// 01C	goes into unk004, Null goes into unk008, 00C and 010
	};

	struct Data06 // Applies to references whose changeFlags bit3 (CHANGE_REFR_CELL_CHANGED) is set
	{
		EncodedID	cellOrWorldspace;		// 000	goes into unk000
		float		posX;					// 003	goes into unk014
		float		posY;					// 007	goes into unk018, associated with unk003
		float		posZ;					// 00B	goes into unk01C, associated with unk003	(pos?)
		float		rotX;					// 00F	goes into unk020
		float		rotY;					// 013	goes into unk024, associated with unk00F
		float		rotZ;					// 017	goes into unk028, associated with unk00F	(rot?)
		EncodedID	newCellOrWorldspaceID;	// 01C	goes into unk008
		SInt16		coordX;					// 01E	goes into unk00C
		SInt16		coordY;					// 020	goes into unk010, Null goes into unk004 and byt02C
	};

	struct Data00  // Every other cases (no data)
	{
	};

	union Data
	{
		Data00	data00;
		Data01	data01;
		Data02	data02;
		Data03	data03;
		Data04	data04;
		Data05	data05;
		Data06	data06;
	};

	Data data;	// 00
};

struct PreloadCELLdata	// Unpacked and decoded version of Data01, 02 end 03
{
	UInt32	worldspaceID;	// 000
	SInt32	coordX;			// 004
	SInt32	coordY;			// 008
	UInt32	detachTime;		// 00C
};

struct PreloadREFRdata	// Unpacked and decoded version of Data04, 05 and 06
{
	UInt32	cellOrWorldspaceID;		// 000
	UInt32	baseFormID;				// 004
	UInt32	newCellOrWorldspaceID;	// 008
	SInt32	coordX;					// 00C
	SInt32	coordY;					// 010
	float	posXcoordX;				// 014
	float	posYcoordY;				// 018
	float	posZ;					// 01C
	float	rotX;					// 020
	float	rotY;					// 024
	float	rotZ;					// 028
	UInt8	flg02C;					// 02C
};

union preloadData
{
	PreloadCELLdata	cell;
	PreloadREFRdata	refr;
};

class BGSLoadGameBuffer
{
	BGSLoadGameBuffer();
	~BGSLoadGameBuffer();

	virtual UInt8			GetSaveFormVersion(void);	// replaced in descendant 
	virtual TESForm*		getForm(void);				// only implemented in descendants
	virtual TESObjectREFR*	getREFR(void);				// only implemented in descendants
	virtual Actor*			getActor(void);				// only implemented in descendants

	char*	chunk;			// 004
	UInt32	chunkSize;		// 008
	UInt32	chunkConsumed;	// 00C
};

struct BGSFormChange
{
	UInt32	changeFlags;
	UInt32	unk004;			// Pointer to the changed record or the save record ?
};

struct	BGSSaveLoadChangesMap
{
	NiTPointerMap<BGSFormChange> BGSFormChangeMap;
	// most likely more
};

// 030
class BGSLoadFormBuffer: public BGSLoadGameBuffer
{
	BGSLoadFormBuffer();
	~BGSLoadFormBuffer();

	typedef UInt8 EncodedID[3];
	struct Header	// 00C
	{
		EncodedID	encodeID;			// 00
		UInt32		changeFlags;		// 03
		UInt8		codedTypeAndLength;	// 07
		UInt8		formVersion;		// 08
		UInt8		pad009[3];			// 09
	};

	UInt32			refID;				// 010
	Header			header;				// 014
	UInt32			bufferSize;			// 020
	TESForm*		form;				// 024
	UInt32			flg028;				// 028	bit1 form invalid
	BGSFormChange*	currentFormChange;	// 02C
};

class BGSSaveFormBuffer: public BGSLoadGameBuffer
{
	BGSSaveFormBuffer();
	~BGSSaveFormBuffer();

};	// in BGSSaveGameBuffer there is a 010, which look like a counter (ChunkCount ?), then the Header

// 1C8 - only explicitly marked things are verified
class TESSaveLoadGame
{
public:
	TESSaveLoadGame();
	~TESSaveLoadGame();

	struct CreatedObject {
		UInt32			refID;
		CreatedObject	* next;
	};

	ChangesMap*						changesMap;			// 000
	UInt32							unk004;				// 004
	InteriorCellNewReferencesMap*	intRefMap;			// 008
	ExteriorCellNewReferencesMap*	extRefMap00C;		// 00C
	ExteriorCellNewReferencesMap*	extRefMap010;		// 010
	void*							saveLoadBuffer;		// 014
	UInt32							unk018;				// 018
	UInt8							unk01C;				// 01C
	UInt8							pad01D[3];
	NiTArray<TESObjectREFR*>*		arr020;				// 020
	UInt32							unk024;				// 024
	UInt32							unk028;				// 028
	CreatedObject					createdObjectList;	// 02C data is formID - verified
	NiTPointerMap<void*>*			map034;				// 034
	UInt32							unk034[(0x58-0x44) >> 2];	// 044
	NumericIDBufferMap*				idMap058;			// 058
	NumericIDBufferMap*				idMap05C;			// 05C
	NumericIDBufferMap*				idMap060;			// 060
	NumericIDBufferMap*				idMap064;			// 064
	UInt32							unk068;				// 068
	UInt32							unk06C;				// 06C
	UInt32							unk070;				// 070
	UInt8							unk074;				// 074
	UInt8							unk075;				//     init to 0x7D
	UInt8							pad076[2];
	NiTArray<UInt32>*				array078;			// 078 NiTLargePrimitiveArray<?>
	NiTArray<UInt32>*				array07C;			// 07C NiTLargePrimitiveArray<?>	
	UInt8							unk080;				// 080 version of save?
	UInt8							unk081;
	UInt8							pad082[2];
	UInt32							unk084[(0xAC-0x84) >> 2];	// 084
	UInt8							unk0AC;				// 0AC
	UInt8							unk0AD;				// 0AD
	UInt8							unk0AE;				// 0AE
	UInt8							unk0AF;				// 0AF
	UInt32							unk0B0[(0x1C8-0x0B0) >> 2];	// 0B0

	static TESSaveLoadGame* Get() { return reinterpret_cast<TESSaveLoadGame*>(0x011DE45C); }

	MEMBER_FN_PREFIX(TESSaveLoadGame);
#if RUNTIME
	DEFINE_MEMBER_FN(AddCreatedForm, UInt32, 0x00861780, TESForm * pForm);
#endif
};

template <typename T_Key, typename T_Data>
class NiTMap : public NiTMapBase<T_Key, T_Data>
{
public:
	NiTMap();
	~NiTMap();
};

class BGSCellNumericIDArrayMap;
class BGSLoadGameSubBuffer;
class BGSReconstructFormsInFileMap;
class BGSReconstructFormsInAllFilesMap;

class BGSSaveLoadGame	// 0x011DDF38
{
public:
	BGSSaveLoadGame();
	~BGSSaveLoadGame();

	typedef UInt32	RefID;
	typedef UInt32	IndexRefID;
	struct RefIDIndexMapping	// reversible map between refID and loaded form index
	{
		NiTMap<RefID, IndexRefID>	* map000;	// 000
		NiTMap<IndexRefID, RefID>	* map010;	// 010
		UInt32			            countRefID;	// 020
	};

	struct SaveChapters	// 06E	chapter table in save
	{
		struct RefIDArray	// List of all refID referenced in save for tranlation in RefIDIndexMapping
		{
			UInt32	count;	// 000
			RefID	IDs[1];	// 004
		};

		RefIDArray	*arr000;	// 000
		RefIDArray	*arr004;	// 004

	};

	struct Struct010
	{
		NiTPointerMap<UInt32>						* map000;	// 000
		BGSCellNumericIDArrayMap					* map010;	// 010
		NiTPointerMap<BGSCellNumericIDArrayMap*>	* map020;	// 020
	};

	BGSSaveLoadChangesMap					* changesMap;			// 000
	BGSSaveLoadChangesMap					* previousChangeMap;	// 004
	RefIDIndexMapping						* refIDmapping;			// 008
	RefIDIndexMapping						* visitedWorldspaces;	// 00C
	Struct010								* sct010;				// 010
	NiTMap<TESForm *, BGSLoadGameSubBuffer> * maps014[3];			// 014	0 = changed Animations, 2 = changed Havok Move
	NiTMap<UInt32, UInt32>					* map018;				// 018	
	BSSimpleArray<char *>					* strings;				// 01C
	BGSReconstructFormsInAllFilesMap*		rfiafMap;				// 020
	BSSimpleArray<BGSLoadFormBuffer *>		changedForms;			// 024
	NiTPointerMap<Actor*>					map0034;				// 034 Either dead or not dead actors
	UInt8									saveMods[255];			// 044
	UInt8									loadedMods[255];		// 143

	UInt16									pad242;					// 242
	UInt32									flg244;					// 244 bit 6 block updating player position/rotation from save, bit 2 set during save
	UInt8									formVersion;			// 248
	UInt8									pad249[3];				// 249

};

class SaveGameManager
{
public:
	SaveGameManager();
	~SaveGameManager();

	struct SaveGameData
	{
		const char	* name;		// 00
		UInt32		unk04;		// 04
		UInt32		saveNumber;	// 08 index?
		const char	* pcName;	// 0C
		const char	* pcTitle;	// 10
		const char	* location;	// 14
		const char	* time;		// 18
	};

	TList<SaveGameData>		* saveList;		// 00
	UInt32					numSaves;		// 04
	UInt32					unk08;			// 08
	UInt8					unk0C;			// 0C	flag for either opened or writable or useSeparator (|)
	UInt8					unk0D;
	UInt8					unk0E;
	UInt8					unk0F;
/*
	const char				* unk10;		// 10 name of most recently loaded/saved game?
	UInt32					unk14;			// 14 init to -1
	UInt8					unk18;			// 18
	UInt8					pad19[3];
	UInt8					unk20;			// 20 init to 1
	UInt8					unk21;
	UInt8					pad22[2];
	UInt32					unk24;			// 24
	UInt32					unk28;			// 28
*/
	static SaveGameManager*		GetSingleton() { return *reinterpret_cast<SaveGameManager**>(0x011DE134); }
	__forceinline UInt32		ConstructSavegameFilename(const char* filename, char* outputBuf, bool bTempFile) { return StdCall<UInt32>(0x0084FF90, filename, outputBuf, bTempFile); }
	__forceinline bool			ConstructSavegamePath(char* outputBuf) { return StdCall<bool>(0x0084FF30, outputBuf); }
};

std::string GetSavegamePath();

class ButtonIcon;

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

class ConsoleManager
{
public:
	ConsoleManager();
	~ConsoleManager();

	struct TextNode
	{
		TextNode*			next;
		TextNode*			prev;
		String				text;
	};

	struct TextList
	{
		TextNode*			first;
		TextNode*			last;
		UInt32				count;

		TextList*			Append(TextNode*);
	};

	struct RecordedCommand
	{
		char				buf[100];
	};

	void*					scriptContext;
	TextList				printedLines;
	TextList				inputHistory;
	unsigned int			historyIndex;
	unsigned int			unk020;
	unsigned int			printedCount;
	unsigned int			unk028;
	unsigned int			lineHeight;
	int						textXPos;
	int						textYPos;
	UInt8					isConsoleOpen;
	UInt8					unk39;
	UInt8					isBatchRecording;
	UInt8					unk3B;
	unsigned int			numRecordedCommands;
	RecordedCommand			recordedCommands[20];
	char					scofPath[260];
	
	__forceinline static ConsoleManager*	GetSingleton(bool canCreateNew = true) { return CdeclCall<ConsoleManager*>(0x0071B160, canCreateNew); }
	void									AppendToSentHistory(const char*);
	__forceinline void						Print(const char* fmt, va_list args) { ThisCall(0x0071D0A0, this, fmt, args); }
	static char*							GetConsoleOutputFilename(void);
	static bool								HasConsoleOutputFilename(void);
};

__forceinline NiTPointerMap<TESForm>* GetAllForms() { return *reinterpret_cast<NiTPointerMap<TESForm>**>(0x11C54C0); }
__forceinline TESForm* GetFormByID(const char* editorID) { return CdeclCall<TESForm*>(0x483A00, editorID); };
__forceinline TESForm* GetFormByID(UInt32 refID) { return GetAllForms()->Lookup(refID); };
TESForm* GetFormByID(const char* mod, UInt32 refID);

typedef void (*EventHandler)(TESObjectREFR* thisObj, void* parameters);

typedef bool				(*_ExtractArgsEx)(COMMAND_ARGS_EX, ...);
typedef bool				(*_ExtractFormatStringArgs)(UInt32 fmtStringPos, char* buffer, COMMAND_ARGS_EX, UInt32 maxParams, ...);
typedef bool				(*_HasScriptCommand)(Script* script, CommandInfo* info, CommandInfo* eventBlock);
typedef CommandInfo*		(*_GetByOpcode)(UInt32 opcode);
typedef const char*			(*_GetStringVar)(UInt32 var);
typedef void				(*_SetStringVar)(UInt32, const char*);
typedef bool				(*_AssignString)(ParamInfo*, void*, TESObjectREFR*, TESObjectREFR*, Script*, ScriptEventList*, double*, UInt32*, const char*);
typedef	bool				(*_SetNativeEventHandler)(const char* eventName, EventHandler func);
typedef bool				(*_RemoveNativeEventHandler)(const char* eventName, EventHandler func);
typedef bool				(*_RegisterEvent)(const char* name, UInt8 numParams, SInt8* paramTypes, UInt32 flags);
typedef bool				(*_DispatchEvent)(const char* eventName, TESObjectREFR* thisObj, ...);
typedef bool				(*_CallFunctionAlt)(Script* funcScript, TESObjectREFR* callingObj, UInt8 numArgs, ...);
typedef Script*				(*_CompileScript)(const char* scriptText);
typedef Script*				(*_CompileExpression)(const char* expression);

inline _ExtractArgsEx				ExtractArgsEx;
inline _ExtractFormatStringArgs		ExtractFormatStringArgs;
inline _HasScriptCommand			HasScriptCommand;
inline _GetByOpcode					GetByOpcode;
inline _GetStringVar				GetStringVar;
inline _SetStringVar				SetStringVar;
inline _AssignString				AssignString;
inline _SetNativeEventHandler		SetNativeEventHandler;
template <typename T> void __forceinline SetEventHandler(const char* eventName, T func)
{ SetNativeEventHandler(eventName, reinterpret_cast<EventHandler>(func)); }
inline _RemoveNativeEventHandler	RemoveNativeEventHandler;
template <typename T> void __forceinline RemoveEventHandler(const char* eventName, T func)
{ RemoveNativeEventHandler(eventName, reinterpret_cast<EventHandler>(func)); }
inline _RegisterEvent				RegisterEvent;
inline _DispatchEvent				DispatchEvent;
inline _CallFunctionAlt				CallFunctionAlt;
inline _CompileScript				CompileScript;
inline _CompileExpression			CompileExpression;
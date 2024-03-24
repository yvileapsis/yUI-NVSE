#pragma once
#include <NiTypes.h>
#include <NiObjects.h>
#include <Containers.h>
#include <BaseFormComponent.h>

// 018
class TESForm : public BaseFormComponent
{
public:

	enum
	{
		kFormFlags_Unk00000002			= 0x00000002,
		kFormFlags_Initialized			= 0x00000008,	// set by TESForm::InitItem()
		kFormFlags_Deleted				= 0x00000020,	// refr removed from .esp or savegame
		kFormFlags_Taken				= kFormFlags_Deleted | kFormFlags_Unk00000002,
		kFormFlags_CastShadows			= 0x00000200,
		kFormFlags_Persistent			= 0x00000400,	//shared bit with kFormFlags_QuestItem
		kFormFlags_QuestItem			= 0x00000400,
		kFormFlags_IsPermanent			= 0x00000800,
		kFormFlags_DontSaveForm			= 0x00004000,	// TODO: investigate
		kFormFlags_Temporary			= 0x00004000,
		kFormFlags_Compressed			= 0x00040000,
		kFormFlags_IgnoreFriendlyHits	= 0x00100000,
		kFormFlags_Destroyed			= 0x00800000,
		kChanged_Inventory				= 0x08000000,
	};

	TESForm();
	virtual				~TESForm();;						// func_00C in GECK ?? I think ??
	virtual void		SetToDefault();						// Might be set default value (called from constructor)
	virtual void		Unk_06();						// Might be clear values
	virtual bool		Unk_07();
	virtual bool		LoadForm(ModInfo * modInfo);		// func_010 in GECK
	virtual bool		Unk_09(void * arg);					// points to LoadForm on TESForm
	virtual bool		AppendForm(ModInfo* modInfo);		// (ie SaveForm + append to modInfo)
	virtual void		SaveForm();						// saves in same format as in .esp	//	func_013 in GECK
															// data buffer and buffer size stored in globals when done, doesn't return anything
	virtual bool		LoadForm2(ModInfo * modInfo);		// just calls LoadForm
	virtual void		WriteFormInfo(ModInfo* modInfo);	// does some saving stuff, then calls Fn0A
	virtual bool		Unk_0E(void * arg);					// prepares a GRUP formInfo
	virtual bool		Sort(TESForm * form);				// returns if the argument is "greater or equal" to this form
	virtual TESForm*	CreateForm(void * arg0, void * mapToAddTo);	// makes a new form, 
	virtual void		Unk_11(void * arg);
	virtual void		MarkAsModified(UInt32 changedFlags);		// enable changed flag?
	virtual void		MarkAsUnmodified(UInt32 changedFlags);		// disable changed flag?
	virtual UInt32		GetSaveSize(UInt32 changedFlags);	// bytes taken by the delta flags for this form, UNRELIABLE, not (always) overriden
	virtual void		AppendSaveLoadFormBuffer(void* arg);					// collect referenced forms?
	virtual void		SaveGame(UInt32 changedFlags);		// Used as part of CopyFromBase with LoadGame.
	virtual void		LoadGame(void * arg);				// load from BGSLoadFormBuffer arg
	virtual void		LoadGame2(UInt32 changedFlags);		// load from TESSaveLoadGame
	virtual void		InitLoadGame(void* arg);
	virtual void		Unk_1A(void * arg0, void * arg1);
	virtual void		Unk_1B(void * arg0, void * arg1);
	virtual void		Revert(UInt32 changedFlags);		// reset changes in form
	virtual void		Unk_1D(void * arg);
	virtual void		Unk_1E(void * arg);
	virtual bool		Unk_1F(void * arg);
	virtual void		Unk_20(void * arg);
	virtual void		Unk_21(void * arg);
	virtual void		InitItem();
	virtual UInt32		GetTypeID();
	virtual void		GetDebugName(String * dst);
	virtual bool		IsQuestItem();
	virtual bool		HasTalkedToPC();		// 00000040
	virtual bool		GetHavokDeath();		// 00010000
	virtual bool		GetFlagBit16();		// 00010000
	virtual bool		GetFlagsBit17();		// 00020000
	virtual bool		GetFlagsBit17_2();		// 00020000
	virtual bool		GetFlagBit19();		// 00080000
	virtual bool		GetFlagBit25();		// 02000000
	virtual bool		GetFlagBit30();		// 40000000
	virtual bool		GetFlagBit9();		// 00000200
	virtual void		UpdateFlagBit9(bool set);	// 00000200
	virtual bool		Unk_30();		// returns false
	virtual void		MarkProjectileForDeletion(bool set);	// 00000020 then calls Fn12 MarkAsModified
	virtual void		SetAltered(bool set);	// 00000002 with a lot of housekeeping
	virtual void		SetQuestItem(bool set);	// 00000400 then calls Fn12 MarkAsModified
	virtual void		SetTalkedToPC(bool set);	// 00000040 then calls Fn12 MarkAsModified
	virtual void		SetHavokDeath(bool set);	// 00010000 then calls Fn12 MarkAsModified
	virtual void		SetNeedToChangeProcess(bool set);	// 00020000
	virtual void		Unk_37();		// write esp format
	virtual void		readOBNDSubRecord(ModInfo * modInfo);	// read esp format
	virtual bool		Unk_39();
	virtual bool		IsBoundObject() const;
	virtual bool		IsMagicItem() const;// EnchantmentItem, SpellItem, IngredientItem, AlchemyItem
	virtual bool		GetIsReference() const;
	virtual bool		IsArmorAddon();
	virtual bool		Unk_3E();
	virtual bool		Unk_3F();	// returnTrue for refr whose baseForm is a TESActorBase
	virtual bool		IsActor() const;
	virtual UInt32		Unk_41();
	virtual void		CopyFrom(const TESForm * form);
	virtual bool		Compare(TESForm * form);
	virtual bool		CheckFormGRUP(void * arg);	// Checks the group is valid for the form
	virtual void		InitFormGRUP(void * dst, void * arg1);	// Fills the groupInfo with info valid for the form
	virtual bool		Unk_46();
	virtual bool		Unk_47();
	virtual bool		Unk_48(UInt32 formType);	// returns if the same FormType is passed in
	virtual bool		Activate(void * arg0, void * arg1, void * arg2, void * arg3, void * arg4);	// looks to be func33 in Oblivion
	virtual void		SetRefID(UInt32 refID, bool generateID);
	virtual char*		GetName2();	// GetName as in OBSE ?
	virtual char*		GetName() const;	// GetEditorID as in OBSE ?
	virtual bool		SetEditorID(const char * edid);		// simply returns true at run-time
	// 4E

	struct EditorData {
		String		editorID;			// 00
		UInt32		vcMasterFormID;		// 08 - Version control 1 (looks to be a refID inside the Version Control master)
		UInt32		vcRevision;			// 0C
	};
	// 10

	enum
	{
		kModified_Flags =	0x00000001
		//	UInt32	flags;
	};

	UInt8		typeID;				// 004
	UInt8		jipFormFlags1;		// 005
	UInt8		jipFormFlags2;		// 006
	UInt8		jipFormFlags3;		// 007


	enum JIPFormFlags
	{
		kJIPFormFlag1_FastTravelInformed = 1 << 0,
		kJIPFormFlag1_CellChangeInformed = 1 << 1,
		kJIPFormFlag1_MenuInputInformed = 1 << 2,
		kJIPFormFlag1_ScriptInformed = kJIPFormFlag1_FastTravelInformed | kJIPFormFlag1_CellChangeInformed | kJIPFormFlag1_MenuInputInformed,
		kJIPFormFlag1_ScriptOnWait = 1 << 3,

		kJIPFormFlag2_MessageDisabled = 1 << 0,
		kJIPFormFlag2_SetStageHandlers = kJIPFormFlag2_MessageDisabled,
		kJIPFormFlag2_ProjectileImpact = kJIPFormFlag2_MessageDisabled,
		kJIPFormFlag2_OnEquipHandlers = kJIPFormFlag2_MessageDisabled,
		kJIPFormFlag2_NoPCActivation = 1 << 1,
		kJIPFormFlag2_EventDisabled = 1 << 2,
		kJIPFormFlag2_ActivateDisabled = 1 << 3,
		kJIPFormFlag2_UniqueItem = 1 << 4,
		kJIPFormFlag2_IsAshPile = 1 << 5,
		kJIPFormFlag2_InsertNode = 1 << 6,
		kJIPFormFlag2_AttachModel = 1 << 7,
		kJIPFormFlag2_InsertObject = kJIPFormFlag2_InsertNode | kJIPFormFlag2_AttachModel,
	};

	UInt32		flags;				// 008
	union
	{
		UInt32		refID;
		struct
		{
			UInt8	id[3];
			UInt8	modIndex;
		};
	};

	TList<ModInfo>	mods;

	TESForm*					TryGetREFRParent() const;
	TESFullName*				GetFullName();
	const char*					GetTheName();
	bool						IsCloned() const { return this->modIndex == 0xFF; }
	Script*						GetScript();
	bool						GetScriptAndEventList(Script*& script, ScriptEventList*& eventList);
	bool						IsItemPlayable();
	UInt32						GetItemValue();
	float						GetWeight();
	float						GetModifiedWeight();
	UInt8						GetOverridingModIdx();
	const char*					GetDescriptionText();
	const char*					RefToString();
	TESLeveledList*				GetLvlList();
	void						SetJIPFlag(UInt8 jipFlag, bool bSet);
	bool						IsQuestItem2() const	{ return flags & kFormFlags_QuestItem; };
	bool						IsDisabled() const	{ return flags & kFormFlags_IsPermanent; };
	bool						IsDeleted() const { return flags & kFormFlags_Deleted; };
	bool						IsInteractionDisabled() const { return jipFormFlags2 & kJIPFormFlag2_NoPCActivation; };
	static void					DoAddForm(TESForm* newForm, bool bPersist = true, bool record = true);
	TESForm*					CloneForm(bool bPersist = true) const;
	bool						IsInventoryObject() { return CdeclCall<char>(0x00481F30, typeID); }
	bool						IsInventoryObjectAlt(); 
	bool						IsCreated() const { return modIndex == 0xFF; }
	__forceinline bool			CanContainItems() const { return typeID == kFormType_TESObjectCONT || typeID == kFormType_TESNPC || typeID == kFormType_TESCreature; }
	__forceinline bool			IsActivator() const { return typeID == kFormType_TESObjectACTI; }


	__forceinline static NiTPointerMap<TESForm>* GetAll() { return *reinterpret_cast<NiTPointerMap<TESForm>**>(0x11C54C0); }
	__forceinline static TESForm* GetByID(const char* editorID) { return CdeclCall<TESForm*>(0x483A00, editorID); };
	__forceinline static TESForm* GetByID(UInt32 refID) { return GetAll()->Lookup(refID); };
	static TESForm* GetByID(const char* mod, UInt32 refID);
	static TESForm* GetByID(const char* mod, const char* refID);
};
static_assert(sizeof(TESForm) == 0x18);


// 24
class EffectItem
{
public:
	EffectItem();
	~EffectItem();

	enum
	{
		kRange_Self = 0,
		kRange_Touch,
		kRange_Target,
	};

	struct ScriptEffectInfo
	{
		UInt32 scriptRefID;
		UInt32 school;
		String effectName;
		UInt32 visualEffectCode;
		UInt32 isHostile;

		void SetName(const char* name);
		void SetSchool(UInt32 school);
		void SetVisualEffectCode(UInt32 code);
		void SetIsHostile(bool bIsHostile);
		bool IsHostile() const;
		void SetScriptRefID(UInt32 refID);

		ScriptEffectInfo* Clone() const;
		void CopyFrom(const ScriptEffectInfo* from);
		static ScriptEffectInfo* Create();
	};

	// mising flags
	UInt32				magnitude;			// 00	used as a float
	UInt32				area;				// 09
	UInt32				duration;			// 08
	UInt32				range;				// 0C
	UInt32				actorValueOrOther;	// 10
	EffectSetting*		setting;	// 14
	float				cost;				// 18 on autocalc items this seems to be the cost
	TList<Condition>	conditions;			// 1C

	//bool HasActorValue() const;
	//UInt32 GetActorValue() const;
	//bool IsValidActorValue(UInt32 actorValue) const;
	//void SetActorValue(UInt32 actorValue);

	//bool IsScriptedEffect() const;
	//UInt32 ScriptEffectRefId() const;
	//UInt32 ScriptEffectSchool() const;
	//UInt32 ScriptEffectVisualEffectCode() const;
	//bool IsScriptEffectHostile() const;

	//EffectItem* Clone() const;
	//void CopyFrom(const EffectItem* from);
	//static EffectItem* Creator();
	//static EffectItem* ProxyEffectItemFor(UInt32 effectCode);
	//
	//bool operator<(EffectItem*rhs) const;
	//// return the magicka cost of this effect item
	//// adjust for skill level if actorCasting is used
	//float MagickaCost(TESForm* actorCasting = NULL) const;

	//void SetMagnitude(UInt32 magnitude);
	//void ModMagnitude(float modBy);
	//void SetArea(UInt32 area);
	//void ModArea(float modBy);
	//void SetDuration(UInt32 duration);
	//void ModDuration(float modBy);
	//void SetRange(UInt32 range);
	//bool IsHostile() const;
	__forceinline int	GetSkillCode() { return ThisCall<signed int>(0x403EA0, this); };
};

// 034
class MagicItemForm : public TESForm
{
public:
	MagicItemForm();

	~MagicItemForm() override;
	virtual void	ByteSwap(); // pure virtual

	// base
	MagicItem	magicItem;	// 018
};

static_assert(sizeof(MagicItemForm) == 0x34);

class ActorValueOwner
{
public:
	ActorValueOwner();
	~ActorValueOwner();

	virtual SInt32	GetBaseActorValueInt(UInt32 avCode);
	virtual Float32	GetBaseActorValue(UInt32 avCode);
	virtual SInt32	GetActorValueInt(UInt32 avCode);
	virtual Float32	GetActorValue(UInt32 avCode);
	virtual Float32	GetBaseSubAV(UInt32 avCode);
	virtual Float32	GetSubAVDamage(UInt32 avCode);
	virtual Float32	GetSubAVMod(UInt32 avCode);
	virtual SInt32	GetPermanentActorValueInt(UInt32 avCode);
	virtual Float32	GetPermanentActorValue(UInt32 avCode);
	virtual Actor*	GetActor();
	virtual UInt16	GetLevel();

	// SkillsCurrentValue[14] at index 20
};

static_assert(sizeof(ActorValueOwner) == 0x004);

class CachedValuesOwner
{
public:
	CachedValuesOwner();
	~CachedValuesOwner();

	virtual float	Fn_00();
	virtual float	Fn_01();
	virtual float	Fn_02();
	virtual float	Fn_03();
	virtual float	Fn_04();
	virtual float	Fn_05();
	virtual float	Fn_06();
	virtual float	Fn_07();
	virtual float	Fn_08();
	virtual float	Fn_09();
	virtual float	Fn_0A();
	virtual UInt32	Fn_0B();
	virtual UInt32	Fn_0C();
	virtual float	Fn_0D();
	virtual float	Fn_0E();
	virtual bool	Fn_0F();
};

static_assert(sizeof(CachedValuesOwner) == 0x004);

// 8
// ### derives from NiObject
class BSTextureSet
{
public:
	BSTextureSet();
	~BSTextureSet();

	void*	_vtbl;	// 0
	UInt32	unk04;		// 4
};

// 4+
class BGSQuestObjective
{
public:
	BGSQuestObjective();
	~BGSQuestObjective();

	enum {
		eQObjStatus_displayed = 1,
		eQObjStatus_completed = 2,
	};

	struct TargetData 
	{
		TESObjectREFR*	target;
		UInt8			flags;
		UInt8			filler[3];
	};

	struct ParentSpaceNode;
	struct TeleportLink
	{
		TESObjectREFR*	door;
		UInt32			unk004[3];
	};	// 016

	struct Target
	{
		UInt8							byt000;				// 000
		UInt8							fill[3];			// 001
		TList<Condition*>				conditions;			// 004
		TESObjectREFR*					target;				// 00C
		BSSimpleArray<ParentSpaceNode>	parentSpaceNodes;	// 010 - The four fields coud be a struct
		BSSimpleArray<TeleportLink>		teleportLinks;		// 020
		UInt32							unk030;				// 030
		UInt32							unk034;				// 034
	};

	UInt32			objectiveId;	// 004 Objective Index in the GECK
	String			displayText;	// 008
	TESQuest*		quest;			// 010
	TList<Target*>	targets;		// 014
	UInt32			unk01C;			// 01C
	UInt32			status;			// 020	bit0 = displayed, bit 1 = completed. 1 and 3 significant. If setting it to 3, quest flags bit1 will be set also.

	virtual void *	Destroy(bool noDealloc);
};

class BGSOpenCloseForm
{
public:
	virtual void	Unk_00(UInt32 arg0, UInt32 arg1);
	virtual void	Unk_01(UInt32 arg0, UInt32 arg1);
	virtual bool	Unk_02();

	BGSOpenCloseForm();
	~BGSOpenCloseForm();
};

/**** forms ****/

class TESTopic;


class TESIdleForm : public TESForm
{
public:
	TESIdleForm();
	~TESIdleForm();

	enum {
		eIFgf_groupIdle = 0,
		eIFgf_groupMovement = 1,
		eIFgf_groupLeftArm = 2,
		eIFgf_groupLeftHand = 3,
		eIFgf_groupLeftWeapon = 4,
		eIFgf_groupLeftWeaponUp = 5,
		eIFgf_groupLeftWeaponDown = 6,
		eIFgf_groupSpecialIdle = 7,
		eIFgf_groupWholeBody = 20,
		eIFgf_groupUpperBody = 20,

		eIFgf_flagOptionallyReturnsAFile = 128,
		eIFgf_flagUnknown = 64,
	};

	struct Data {
		UInt8			groupFlags;		// 000	animation group and other flags
		UInt8			loopMin;		// 001
		UInt8			loopMax;		// 002
		UInt8			fil03B;			// 003
		UInt16			replayDelay;	// 004
		UInt8			flags;			// 006	bit0 is No attacking
		UInt8			fil03F;			// 007
	};

	TESModelAnim		anim;			// 018
	TList<Condition*>	conditions;		// 030
	Data				data;			// 038
	UInt32				unk040;			// 040	NiFormArray, contains all idle anims in path if eIFgf_flagUnknown is set
	TESIdleForm			* parent;		// 044
	TESIdleForm			* previous;		// 048
	String				str04C;			// 04C
};

struct TESTopicInfoResponse
{
	struct Data
	{
		UInt32	emotionType;	//	00
		UInt32	emotionValue;	//	04	Init'd to 0x32
		UInt32	unused;			//	08
		UInt8	responseNumber;	//	0C
		UInt8	pad00D[3];		
		UInt32	sound;			//	10
		UInt8	flags;			//	14	Init'd to 1
		UInt8	pad015[3];
	};

	Data			data;					//	000
	String			responseText;			//	018
	TESIdleForm	*	spkeakerAnimation;		//	020
	TESIdleForm	*	listenerAnimation;		//	024
	TESTopicInfoResponse	* next;			//	028
};

// 50
class TESTopicInfo : public TESForm
{
public:
	TESTopicInfo();
	~TESTopicInfo();

	struct RelatedTopics
	{
		TList<TESTopic*>	linkFrom;
		TList<TESTopic*>	choices;
		TList<TESTopic*>	unknown;
	};

	TList<Condition*>	conditions;			// 18
	UInt16				unk20;				// 20
	UInt8				saidOnce;			// 22
	UInt8				type;				// 23
	UInt8				nextSpeaker;		// 24
	UInt8				flags1;				// 25
	UInt8				flags2;				// 26
	UInt8				pad27;				// 27
	String				prompt;				// 28
	TList<TESTopic*>	addTopics;			// 30
	RelatedTopics	*	relatedTopics;		// 38
	UInt32				speaker;			// 3C
	UInt32				actorValueOrPerk;	// 40
	UInt32				speechChallenge;	// 44
	UInt32				unk48;				// 48
	UInt32				modInfoFileOffset;	// 4C	during LoadForm
};

typedef NiTArray<TESTopicInfo*> TopicInfoArray;
typedef void* INFO_LINK_ELEMENT;

// 40
class TESTopic : public TESForm
{
public:
	TESTopic();
	~TESTopic();

	struct Info	//	34
	{
		TESQuest		*	quest;		//	00
		TopicInfoArray	infoArray;	//	04
		BSSimpleArray<INFO_LINK_ELEMENT>		unk01C;
		TESQuest		*	quest2;		//	2C
		UInt8			unk030;
		UInt8			pad031[3];
	};

	TESFullName		fullName;	// 18

	UInt8			type;			// 24	DATA
	UInt8			flags;			// 25	DATA also used as bool or flag, connected to INFOGENERAL
	UInt8			pad26[2];		// 26
	float			priority;		// 28	PNAM
	TList<Info*>	infos;			// 2C
	String			unk34;			// 34	TDUM
	UInt16			unk3C;			// 3C	XIDX
	UInt16			unk3E;			// 3E
};
static_assert(sizeof(TESTopic) == 0x40);
// 24
class BGSMenuIcon : public TESForm
{
public:
	BGSMenuIcon();
	~BGSMenuIcon();

	TESIcon	icon;	// 18
};

static_assert(sizeof(BGSMenuIcon) == 0x24);

// 28
class TESGlobal : public TESForm
{
public:
	TESGlobal();
	~TESGlobal();

	enum
	{
		kType_Short =	's',
		kType_Long =	'l',
		kType_Float =	'f'
	};

	String	name;		// 18
	UInt8	type;		// 20
	UInt8	pad21[3];	// 21
	float	data;		// 24
};

static_assert(sizeof(TESGlobal) == 0x28);

// 60
class TESClass : public TESForm
{
public:
	TESClass();
	~TESClass();

	enum
	{
		kFlag_Playable =	0x00000001,
		kFlag_Guard =		0x00000002,
	};

	enum
	{
		kService_Weapons =		0x00000001,
		kService_Armor =		0x00000002,
		kService_Clothing =		0x00000004,
		kService_Books =		0x00000008,
		kService_Food =			0x00000010,
		kService_Chems =		0x00000020,
		kService_Stimpacks =	0x00000040,
		kService_Lights =		0x00000080,	// ??
		kService_Misc =			0x00000400,
		kService_Potions =		0x00002000,	// probably deprecated
		kService_Training =		0x00004000,
		kService_Recharge =		0x00010000,
		kService_Repair =		0x00020000,
	};

	TESFullName		fullName;		// 18
	TESDescription	description;	// 24
	TESTexture		texture;		// 2C
	TESAttributes	attributes;		// 38

	// corresponds to DATA chunk
	UInt32			tagSkills[4];	// 44
	UInt32			classFlags;		// 54
	UInt32			services;		// 58
	UInt8			teaches;		// 5C
	UInt8			trainingLevel;	// 5D
	UInt8			pad5E[2];		// 5E
};

static_assert(sizeof(TESClass) == 0x60);

class TESReputation : public TESForm
{
};

// 4C
class TESFaction : public TESForm
{
public:
	TESFaction();
	~TESFaction();
	enum
	{
		// TESForm flags

		// TESReactionForm flags

		kModified_FactionFlags =	0x00000004
		// CHANGE_FACTION_FLAGS
		// UInt8	flags;
	};

	enum
	{
		kFlag_HiddenFromPC =	0x00000001,
		kFlag_Evil =			0x00000002,
		kFlag_SpecialCombat =	0x00000004,

		kFlag_TrackCrime =		0x00000100,
		kFlag_AllowSell =		0x00000200,
	};

	// 1C
	struct Rank
	{
		String		name;		// 00
		String		femaleName;	// 08
		TESTexture	insignia;	// 10 - effectively unused, can be set but there is no faction UI
	};

	TESFullName		fullName;	// 18
	TESReactionForm	reaction;	// 24

	UInt32			factionFlags;	// 34
	TESReputation*	reputation;		// 38
	TList <Rank>	ranks;			// 3C
	UInt32			crimeCount44;	// 44
	UInt32			crimeCount48;	// 48

	bool IsFlagSet(UInt32 flag) {
		return (factionFlags & flag) != 0;
	}
	void SetFlag(UInt32 flag, bool bMod) {
		factionFlags = bMod ? (factionFlags | flag) : (factionFlags & ~flag);
		MarkAsModified(kModified_FactionFlags);
	}
	bool IsHidden()
	{	return IsFlagSet(kFlag_HiddenFromPC);	}
	bool IsEvil()
	{	return IsFlagSet(kFlag_Evil);	}
	bool HasSpecialCombat()
	{	return IsFlagSet(kFlag_SpecialCombat);	}
	void SetHidden(bool bHidden)
	{	SetFlag(kFlag_HiddenFromPC, bHidden);	}
	void SetEvil(bool bEvil)
	{	SetFlag(kFlag_Evil, bEvil);	}
	void SetSpecialCombat(bool bSpec)
	{	SetFlag(kFlag_SpecialCombat, bSpec);	}
	const char* GetNthRankName(UInt32 whichRank, bool bFemale = false);
	void SetNthRankName(const char* newName, UInt32 whichRank, bool bFemale);
};

static_assert(sizeof(TESFaction) == 0x4C);

// 50
class BGSHeadPart : public TESForm
{
public:
	BGSHeadPart();
	~BGSHeadPart();

	enum
	{
		kFlag_Playable =	0x01,
	};

	TESFullName			fullName;	// 18
	TESModelTextureSwap	texSwap;	// 24

	UInt8				headFlags;	// 44
	UInt8				pad45[3];	// 45
	UInt32				unk48;		// 48
	UInt32				unk4C;		// 4C
};

static_assert(sizeof(BGSHeadPart) == 0x50);

// 4C
class TESHair : public TESForm
{
public:
	TESHair();
	~TESHair();

	enum
	{
		kFlag_Playable =	0x01,
		kFlag_NotMale =		0x02,
		kFlag_NotFemale =	0x04,
		kFlag_Fixed =		0x08,
	};

	TESFullName		fullName;	// 18
	TESModel		model;		// 24
	TESTexture		texture;	// 3C

	UInt8			hairFlags;	// 48	Playable, not Male, not Female, Fixed
	UInt8			pad49[3];	// 49
};

static_assert(sizeof(TESHair) == 0x4C);

// 34
class TESEyes : public TESForm
{
public:
	TESEyes();
	~TESEyes();

	enum
	{
		kFlag_Playable =	0x01,
		kFlag_NotMale =		0x02,
		kFlag_NotFemale =	0x04,
	};

	TESFullName		fullName;	// 18
	TESTexture		texture;	// 24

	UInt8			eyeFlags;	// 30
	UInt8			pad31[3];	// 31
};

static_assert(sizeof(TESEyes) == 0x34);

// 4E4 - incomplete
class TESRace : public TESForm
{
public:
	// 18
	struct FaceGenData
	{
		UInt32	unk00;
		UInt32	unk04;
		UInt32	unk08;
		UInt32	unk0C;
		UInt32	unk10;
		UInt32	unk14;
	};

	// 2
	struct SkillMod
	{
		UInt8	actorValue;
		SInt8	mod;
	};

	enum
	{
		kFlag_Playable =	0x00000001,
		kFlag_Child =		0x00000004,
	};

	TESRace();
	~TESRace();

	TESFullName		fullName;				// 018
	TESDescription	desc;					// 024
	TESSpellList	spells;					// 02C
	TESReactionForm	reaction;				// 040

	SkillMod		skillMods[7];			// 050
	UInt8			pad05E[2];				// 05E
	float			height[2];				// 060 male/female
	float			weight[2];				// 068 male/female
	UInt32			raceFlags;				// 070

	TESAttributes	baseAttributes[2];		// 074 male/female
	TList<TESHair>	hairs;					// 08C
	TESHair *		defaultHair[2];			// 094 male/female
	UInt8			defaultHairColor[2];	// 09C male/female
	UInt8			fill09E[2];				// 09E

	UInt32			unk0A0[(0xA8 - 0xA0) >> 2];	// 0A0

	TList<TESEyes>	eyes;					// 0A8

	TESModel		faceModels[2][8];			// 0B0	male/female Head, Ears, Mouth, TeethLower, TeethUpper, Tongue, LeftEye, RightEye
	TESTexture		faceTextures[2][8];			// 230	male/female Head, Ears, Mouth, TeethLower, TeethUpper, Tongue, LeftEye, RightEye
	TESTexture		bodyPartsTextures[2][3];	// 2F0	male/female	UpperBody, LeftHand, RightHand
	TESModel		bodyModels[2][3];			// 338	male/female	UpperBody, LeftHand, RightHand
	BGSTextureModel	bodyTextures[2];			// 3C8	male/female	EGT file, not DDS.
	FaceGenData		unk3F8[2][4];				// 3F8  male/female

	UInt32			unk4B8[(0x4CC - 0x4B8) >> 2]; // 4B8

	String				name;				// 4CC
	NiTArray <void *>	faceGenUndo;		// 4D4 - NiTPrimitiveArray<FaceGenUndo *>

	bool IsPlayable() const { return (raceFlags & kFlag_Playable) == kFlag_Playable; }
};

// 60
class TESSkill : public TESForm
{
public:
	TESSkill();
	~TESSkill();

	TESDescription	description;	// 18
	TESTexture		texture;		// 20

	UInt32			unk2C;			// 2C
	UInt32			unk30;			// 30
	UInt32			unk34;			// 34
	float			unk38;			// 38
	float			unk3C;			// 3C
	TESDescription	desc2[3];		// 40
	UInt32			unk58[(0x60 - 0x58) >> 2];	// 58
};

static_assert(sizeof(TESSkill) == 0x60);

// B0
class EffectSetting : public TESForm	// MGEF
{
public:
	EffectSetting();
	~EffectSetting();

	enum
	{
		kArchType_ValueModifier = 0,
		kArchType_Script,
		kArchType_Dispel,
		kArchType_CureDisease,
		kArchType_Absorb,
		kArchType_Shield,
		kArchType_Calm,
		kArchType_Demoralize,
		kArchType_Frenzy,
		kArchType_CommandCreature,
		kArchType_CommandHumanoid,
		kArchType_Invisibility,
		kArchType_Chameleon,
		kArchType_Light,
		kArchType_Darkness,
		kArchType_NightEye,
		kArchType_Lock,
		kArchType_Open,
		kArchType_BoundItem,
		kArchType_SummonCreature,
		kArchType_DetectLife,
		kArchType_Telekinesis,
		kArchType_DisintigrateArmor,
		kArchType_DisinitgrateWeapon,
		kArchType_Paralysis,
		kArchType_Reanimate,
		kArchType_SoulTrap,
		kArchType_TurnUndead,
		kArchType_SunDamage,
		kArchType_Vampirism,
		kArchType_CureParalysis,
		kArchType_CureAddiction,
		kArchType_CurePoison,
		kArchType_Concussion,
		kArchType_ValueAndParts,
		kArchType_LimbCondition,
		kArchType_Turbo,
	};

	enum EffectFlags
	{
		kHostile = 0x1,
		kRecover = 0x2,
		kDetrimental = 0x4,
		kSelf = 0x10,
		kTouch = 0x20,
		kTarget = 0x40,
		kNoDuration = 0x80,
		kNoMagnitude = 0x100,
		kNoArea = 0x200,
		kFXPersist = 0x400,
		kGoryVisuals = 0x1000,
		kDisplayNameOnly = 0x2000,
		kRadioBroadcastSomething = 0x8000,
		kUseSkill = 0x80000,
		kUseAttribute = 0x100000,
		kPainless = 0x1000000,
		kSprayProjectileType = 0x2000000,
		kBoltProjectileType = 0x4000000,
		kNoHitEffect = 0x8000000,
		kNoDeathDispel = 0x10000000,
	};

	TESModel				model;					// 18
	TESDescription			description;			// 30
	TESFullName				fullName;				// 38
	TESIcon					icon;					// 44
	UInt32					unk50;					// 50
	UInt32					unk54;					// 54
	UInt32					effectFlags;			// 58 - start of DATA
	float					baseCost;				// 5C
	TESForm*				associatedItem;			// 60 - Script* for ScriptEffects
	UInt32					magicSchool;			// 64 - unused
	UInt32					resistVal;				// 68 - actor value for resistance
	UInt16					counterEffectCount;		// 6C - counter effects count
	UInt8					pad6E[2];				// 6E
	TESObjectLIGH*			light;					// 70
	float					projectileSpeed;		// 74
	TESEffectShader*		effectShader;			// 78 - effect shader
	TESEffectShader*		objectDisplayShader;	// 7C
	TESSound*				effectSound;			// 80
	TESSound*				boltSound;				// 84
	TESSound*				hitSound;				// 88
	TESSound*				areaSound;				// 8C
	float					unused090;				// 90 - fMagicDefaultCEEnchantFactor
	float					unused094;				// 94 - fMagicDefaultCEBarterFactor
	UInt32					archtype;				// 98
	UInt32					actorVal;				// 9C - actor value - last field of DATA
	TList<EffectSetting>	counterEffects;		// A0 - counter effects list
	UInt32					unkA8;					// A8
	UInt32					unkAC;					// AC

	Script*			GetScript() { return 1 == archtype ? (Script*)associatedItem : NULL; };
	bool			HasScript() { return 1 == archtype ? true : false; };
	Script*			RemoveScript();
	Script*			SetScript(Script* newScript);
};

static_assert(sizeof(EffectSetting) == 0xB0);
// 28
class TESLandTexture : public TESForm
{
public:
	TESLandTexture();
	~TESLandTexture();

	BGSTextureSet	*textureSet;		// 18
	UInt8			materialType;		// 1C
	UInt8			friction;			// 1D
	UInt8			restitution;		// 1E
	UInt8			specularExponent;	// 1F
	TList<TESGrass>	grasses;			// 20

	SInt32	GetGrassIndex(TESGrass *grass);
};
static_assert(sizeof(TESLandTexture) == 0x28);

// 44
class EnchantmentItem : public MagicItemForm
{
public:
	EnchantmentItem();
	~EnchantmentItem();

	virtual void	ByteSwap();

	enum
	{
		kType_Weapon = 2,
		kType_Apparel,
	};

	UInt32		type;		// 34
	UInt32		unk38;		// 38
	UInt32		unk3C;		// 3C
	UInt8		enchFlags;	// 40
	UInt8		pad41[3];	// 41
};
static_assert(sizeof(EnchantmentItem) == 0x44);

// 44
class SpellItem : public MagicItemForm
{
public:
	SpellItem();
	~SpellItem();

	virtual void	ByteSwap();

	enum
	{
		kType_ActorEffect	= 0,
		kType_Disease,
		kType_Power,
		kType_LesserPower,
		kType_Ability,
		kType_Poison,
		kType_Addiction		= 10,
	};

	UInt32		type;		// 34
	UInt32	unk38;		// 38	Cost Unused
	UInt32	unk3C;		// 3C	Level Unused
	UInt8		spellFlags;	// 40
	UInt8	pad41[3];	// 41
};

static_assert(sizeof(SpellItem) == 0x44);
// IngredientItem (A4)
class IngredientItem;

// BGSStaticCollection (50)
class BGSStaticCollection;

// BGSMovableStatic (6C)
class BGSMovableStatic;

// BGSPlaceableWater (50)
class BGSPlaceableWater;

// TESObjectTREE (94)
class TESObjectTREE;

// TESFlora (90)
class TESFlora;

// TESFurniture (88)
class TESFurniture;
class TESAmmoEffect : public TESForm
{
public:
	TESAmmoEffect();
	~TESAmmoEffect();

	enum
	{
		kEffectType_DamageMod		=	0,
		kEffectType_DRMod			=	1,
		kEffectType_DTMod			=	2,
		kEffectType_SpreadMod		=	3,
		kEffectType_ConditionMod	=	4,
		kEffectType_FatigueMod		=	5,
	};

	enum
	{
		kOperation_Add		=	0,
		kOperation_Multiply	=	1,
		kOperation_Subtract	=	2,
	};

	TESFullName		fullName;	// 018	
	UInt32			type;		// 024
	UInt32			operation;	// 028
	float			value;		// 02C
}; 
static_assert(sizeof(TESAmmoEffect) == 0x30);
class BSFaceGenNiNode;

// 02B4
struct ValidBip01Names {	// somehow descend from NiNodeArray
	enum eOptionalBoneType {
		kOptionalBone_Bip01Head			= 0,
		kOptionalBone_Weapon			= 1,
		kOptionalBone_Bip01LForeTwist	= 2,
		kOptionalBone_Bip01Spine2		= 3,
		kOptionalBone_Bip01Neck1		= 4,
	};

	// 008
	struct OptionalBone	
	{
		bool	exists;
		NiNode	* bone;
	};

	// 010
	struct Data
	{
		TESForm		* model;	// 000 can be a modelled form (Armor or Weapon) or a Race if not equipped
		TESModel	* texture;	// 004 texture or model for said form
		NiNode		* bones;	// 008 NiNode for the modelled form
		UInt32		unk00C;		// 00C Number , bool or flag
	};

	NiNode			* bip01;		// 000 receive Bip01 node, then optionally Bip01Head, Weapon, Bip01LForeTwist, Bip01Spine2, Bip01Neck1
	OptionalBone	bones[5];		// 004
	Data			unk002C[20];	// 02C indexed by the EquipSlot
	Data			unk016C[20];	// 16C indexed by the EquipSlot
	Character		* character;	// 2B0

	MEMBER_FN_PREFIX(ValidBip01Names);
#if RUNTIME
	DEFINE_MEMBER_FN(Destroy, bool, 0x00418E00, ValidBip01Names*, bool);	// Destoy and optionally free the structure/class
#endif
};

// BGSIdleMarker (40)
class BGSIdleMarker;

// BGSConstructibleObject (B0)
class BGSConstructibleObject;

// TESLevItem (44)
class TESLevItem;
class TESImageSpaceModifier;

class RGBA {
public:
	UInt8	r;
	UInt8	g;
	UInt8	b;
	UInt8	a;
};
static_assert(sizeof(RGBA) == 0x004);

// 2F4
class TESWeather : public TESForm
{
public:
	TESWeather();
	~TESWeather();

	struct ColorData {
		RGBA colors[6];
	};

	UInt32					unk018;						// 018
	TESImageSpaceModifier	*imageSpaceMods[6];			// 01C
	TESTexture				layerTextures[4];			// 034
	UInt8					cloudSpeed[4];				// 064
	UInt32					unk068[24];					// 068
	TESModel				model;						// 0C8
	UInt8					windSpeed;					// 0E0
	UInt8					cloudSpeedLower;			// 0E1
	UInt8					cloudSpeedUpper;			// 0E2
	UInt8					transDelta;					// 0E3
	UInt8					sunGlare;					// 0E4
	UInt8					sunDamage;					// 0E5
	UInt8					precipitationBeginFadeIn;	// 0E6
	UInt8					precipitationEndFadeOut;	// 0E7
	UInt8					lightningBeginFadeIn;		// 0E8
	UInt8					lightningEndFadeOut;		// 0E9
	UInt8					lightningFrequency;			// 0EA
	UInt8					weatherClassification;		// 0EB
	UInt32					lightningColor;				// 0EC
	float					fogDistance[6];				// 0F0
	ColorData				colors[10];				// 108
	UInt32					unk1F8[63];					// 1F8
};
static_assert(sizeof(TESWeather) == 0x2F4);

// 58
class TESClimate : public TESForm
{
public:
	TESClimate();
	~TESClimate();

	TESModel			nightSkyModel;		// 18
	WeatherTypes		weatherTypes;		// 30
	TESTexture			sunTexture;			// 38
	TESTexture			sunGlareTexture;	// 44
	UInt8				sunriseBegin;		// 50
	UInt8				sunriseEnd;			// 51
	UInt8				sunsetBegin;		// 52
	UInt8				sunsetEnd;			// 53
	UInt8				volatility;			// 54
	UInt8				phaseLength;		// 55
	UInt8				pad56[2];			// 56

	WeatherEntry *GetWeatherEntry(TESWeather *weather, bool rmv);
};

static_assert(sizeof(TESClimate) == 0x58);

struct AreaPointEntry
{
	float	x;
	float	y;
};
typedef TList<AreaPointEntry> AreaPointEntryList;

struct RegionAreaEntry
{
	AreaPointEntryList	points;
	UInt32				unk08[2];
	float				unk10[4];
	UInt32				edgeFallOff;
	UInt32				pointCount;
};
typedef TList<RegionAreaEntry> RegionAreaEntryList;

// 38
class TESRegion : public TESForm
{
public:
	TESRegion();
	~TESRegion();

	RegionDataEntryList*	dataEntries;	// 18
	RegionAreaEntryList*	areaEntries;	// 1C
	TESWorldSpace*			worldSpace;		// 20
	TESWeather*				weather;		// 24
	UInt32					unk28[4];		// 28
};

static_assert(sizeof(TESRegion) == 0x38);

class TESRegionList : public BSSimpleList<TESRegion>
{
public:
	TESRegionList();
	~TESRegionList();
};

// NavMeshInfoMap (40)
class NavMeshInfoMap;



struct Condition {
	UInt8			type;		// 000
	UInt8			pad[3];

	union {						// 004
		Float32		value;
		UInt32		global;
	}				comparisonValue;

	UInt32			function;	// 008

	union {						// 00C
		Float32		value;
		UInt32		number;
		void* pointer;
	}				parameter1;

	union {						// 010
		Float32		value;
		UInt32		number;
		void* pointer;
	}				parameter2;

	UInt32			runOn;		// 014	Subject, Target, Reference, CombatTarget, LinkedReference
	TESObjectREFR* reference;	// 018
};
static_assert(sizeof(Condition) == 0x1C);


struct PermanentClonedForm {
	UInt32 orgRefID;
	UInt32 cloneRefID;
};

typedef TList<EffectItem> EffectList;

class TESLeveledList;


// TESObjectCELL (E0)
class TESObjectCELL : public TESForm
{
public:
	TESObjectCELL();
	~TESObjectCELL();

	typedef TList<TESObjectREFR> RefList;
	struct CellCoordinates
	{
		UInt32				x;
		UInt32				y;
	}; // Exterior is 3 DWord, Interior is 5 DWord and 5 floats

	TESFullName				fullName;			// 018	// 030 in GECK
	UInt16					unk024;				// 024	// bit 0 would be interior, that makes it DATA
	UInt8					flags2;				// 026	// 5 or 6 would mean cell is loaded, name based on OBSE
	UInt8					unk027;				// 027
	ExtraDataList			extraDataList;		// 028
	CellCoordinates*		coords;				// 048
	TESObjectLAND*			land;				// 04C
	float					unk050;				// 050
	TESTexture				texture054;			// 054
	void*					NavMeshArray;		// 060	?$BSSimpleArray@VNavMeshPtr@@$0EAA@@@
	UInt32					unk064[(0x0A4-0x064) >> 2];	// 064	080 is CellRefLock semaphore
	UInt32					actorCount;			// 0A4
	UInt16					countVisibleWhenDistant;	// 0A8
	UInt16					unk0AA;				// 0AA
	RefList					objectList;			// 0AC
	NiNode*					niNode0B4;			// 0B4
	NiNode*					niNode0B8;			// 0B8
	UInt32					unk0BC;				// 0BC
	TESWorldSpace*			worldSpace;			// 0C0
	NiNode*					unk0C4;				// 0C4	structure (NiNode) containing at 20 a vector XYZT, 4C a list of scripted references, 5C a list of refer with activateRefChildren
	float					unk0C8;				// 0C8
	UInt32					unk0CC;				// 0CC
	UInt32					unk0D0;				// 0D0
	BSPortalGraph*			portalGraph;		// 0D4
	BGSLightingTemplate*	lightingTemplate;	// 0D8
	UInt32					unk0DC;				// 0DC

	bool IsInterior() { return worldSpace == nullptr; }
};
static_assert(sizeof(TESObjectCELL) == 0xE0);

struct LODdata;	// 03C

// TESWorldSpace (EC)
class TESWorldSpace : public TESForm
{
public:
	TESWorldSpace();
	~TESWorldSpace();

	struct CoordXY {
		float	X;	// 000
		float	Y;	// 004
	};	// 008

	struct DCoordXY {
		SInt32	X;	// 000
		SInt32	Y;	// 004
	};	// 008

	struct WCoordXY {
		SInt16	X;	// 000
		SInt16	Y;	// 002
	};	// 004

	struct Offset_Data
	{
		UInt32**	unk000;		// 000 array of UInt32 stored in OFST record (indexed by relative CellXY).
		CoordXY		min;		// 004 NAM0
		CoordXY		max;		// 00C NAM9
		UInt32		fileOffset;	// 014 TESWorldspace file offset in modInfo
	};	// 014

	struct MapData
	{
		DCoordXY	usableDimensions;
		WCoordXY	cellNWCoordinates;
		WCoordXY	cellSECoordinates;
	};	// 010

	struct ImpactData
	{
		typedef NiTMapBase<BGSImpactData *,BGSImpactData *> ImpactImpactMap;
		enum MaterialType
		{
		  eMT_Stone = 0,
		  eMT_Dirt,
		  eMT_Grass,
		  eMT_Glass,
		  eMT_Metal,
		  eMT_Wood,
		  eMT_Organic,
		  eMT_Cloth,
		  eMT_Water,
		  eMT_HollowMetal,
		  eMT_OrganicBug,
		  eMT_OrganicGlow,

		  eMT_Max
		};

		ImpactImpactMap	impactImpactMap[eMT_Max];	// 000
		char			footstepMaterials[0x12C];	// 030
	};

	typedef NiTPointerMap<BSSimpleList<TESObjectREFR>>	RefListPointerMap;
	typedef NiTPointerMap<TESObjectCELL>				CellPointerMap;
	typedef NiTMapBase<ModInfo *, Offset_Data*>			OffsetDataMap;

	TESFullName			fullName;			// 018 confirmed
	TESTexture			texture;			// 024 confirmed ICON
	CellPointerMap*		cellMap;			// 030 confirmed
	TESObjectCELL*		cell;				// 034 should be the Permanent cell
	UInt32				unk038;				// 038
	LODdata*			lodData;			// 03C looks to be the LOD data (That is what is returned when checking the parent "Use LOD data" flag)
	TESClimate*			climate;			// 040 confirmed CNAM
	TESImageSpace*		imageSpace;			// 044 confirmed INAM
	ImpactData*			impacts;			// 048 confirmed
	UInt8				flags;				// 04C confirmed DATA
	UInt8				unk04D;				// 04D filler
	UInt16				parentFlags;		// 04E init'd to FF if has a parent. 5 is use ImageSpace, 4 is use parent climate, 3 is use parent Water, 1 is use parent LOD data, 0 is use parent LAND data
	RefListPointerMap	pointerMap;			// 050 confirmed
	TList<void*>		lst060;				// 060
	TList<void*>		lst068;				// 068 confirmed as TList
	TESWorldSpace*		parent;				// 070 confirmed
	TESWaterForm*		waterFormFirst;		// 074 confirmed NAM2
	TESWaterForm*		waterFormLast;		// 078 confirmed NAM3 LOD Water type for xEdit
	float				waterHeight;		// 07C confirmed NAM4
	MapData				mapData;			// 080 confirmed MNAM
	float				worldMapScale;		// 090 confirmed ONAM for three floats
	float				worldMapCellX;		// 094 confirmed
	float				worldMapCellY;		// 098 confirmed
	BGSMusicType*		music;				// 09C confirmed ZNAM
	CoordXY				min;				// 0A0 confirmed NAM0 min of all Offset_Data.min
	CoordXY				max;				// 0A8 confirmed NAM9 max of all Offset_data.max
	OffsetDataMap		offsetMap;			// 0B0 guarded by an isESM
	String				str0C0;				// 0C0
	float				defaultLandHeight;	// 0C8 confirmed DNAM for the two
	float				defaultWaterHeight;	// 0CC
	BGSEncounterZone*	encounterZone;		// 0D0 confirmed	
	TESTexture			canopyShadow;		// 0D4 confirmed NNAM
	TESTexture			waterNoiseTexture;	// 0E0 confirmed XNAM
};	// I am seeing a TList at 60, a map at 50 indexed by XY coord !!!, another map at B0, indexed by modInfo::Unklist elements
static_assert(sizeof(TESWorldSpace) == 0xEC);

class TESChildCell
{
public:
	TESChildCell();
	~TESChildCell();

	// no virtual destructor
	virtual TESObjectCELL* GetPersistentCell();		// 000

	//	void	** vtbl;	// 00
};
static_assert(sizeof(TESChildCell) == 0x4);

// TESObjectLAND (2C)
class TESObjectLAND;

struct NavMeshVertex;
struct NavMeshTriangle;
struct EdgeExtraInfo;
struct NavMeshTriangleDoorPortal;
struct NavMeshClosedDoorInfo;
struct NavMeshPOVData;
struct ObstacleUndoData;
struct NavMeshStaticAvoidNode;

class NavMeshInfoMap;

struct NavMeshInfo;

// 8C
class ObstacleData : public NiRefObject
{
public:
	UInt32						unk08;			// 08
	NiRefObject*				object0C;		// 0C
	UInt32						unk10[25];		// 10
	UInt8						byte74;			// 74
	UInt8						byte75[3];		// 75
	BSSimpleArray<NavMeshInfo>	navMeshInfos;	// 78
	NiRefObject*				object88;		// 88
};

// NavMesh (108)
class NavMesh : public TESForm
{
public:
	virtual void		Unk_4E(void);

	TESChildCell								childCell;			// 018
	NiRefObject									refObject;			// 01C
	TESObjectCELL*								parentCell;			// 024
	BSSimpleArray<NavMeshVertex>				vertexArr;			// 028
	BSSimpleArray<NavMeshTriangle>				triangleArr;		// 038
	BSSimpleArray<EdgeExtraInfo>				edgeInfoArr;		// 048
	BSSimpleArray<NavMeshTriangleDoorPortal>	doorPortalArr;		// 058
	BSSimpleArray<NavMeshClosedDoorInfo>		closedDorrArr;		// 068
	BSSimpleArray<UInt16>						unk078Arr;			// 078
	NiTMapBase<UInt16, NavMeshPOVData*>			povDataMap;			// 088
	BSSimpleArray<UInt16>						unk098Arr;			// 098
	UInt32										unk0A8;				// 0A8
	float										unk0AC[8];			// 0AC
	BSSimpleArray<UInt16>*						arrPtr0CC;			// 0CC
	BSSimpleArray<ObstacleUndoData>				obstacleUndoArr;	// 0D0
	NiTMapBase<UInt16, ObstacleData*>*			obstacleDataMap;	// 0E0
	BSSimpleArray<UInt16>						unk0E4Arr;			// 0E4
	BSSimpleArray<NavMeshStaticAvoidNode>		avoidNodeArr;		// 0F4
	UInt32*										ptr104;				// 104
};
static_assert(sizeof(NavMesh) == 0x108);

struct VariableInfo
{
	UInt32			idx;		// 00
	UInt32			pad04;		// 04
	Float64			data;		// 08
	UInt8			type;		// 10
	UInt8			pad11[3];	// 11
	UInt32			unk14;		// 14
	String			name;		// 18
};

// TESQuest (6C)
class TESQuest : public TESForm
{
public:
	TESQuest();
	~TESQuest();

	virtual char * GetEditorName() const;			// 4E

	// bases
	TESScriptableForm			scriptable;			// 018
	TESIcon						icon;				// 024
	TESFullName					fullName;			// 030

	struct StageInfo {
		UInt8			stage;	// 00 stageID
		UInt8			unk001;	// 01 status ?
		UInt8			pad[2];	// 02
		TList<void*>	unk004;	// 04 log entries
	};	// 00C

	union LocalVariableOrObjectivePtr
	{
		BGSQuestObjective	*objective;
		VariableInfo		*varInfoIndex;
	};

	UInt8						flags;						// 03C	bit0 is startGameEnabled/isRunning
	UInt8						priority;					// 03D
	UInt8						pad03E[2];					// 03E
	float						questDelayTime;				// 040
	TList<StageInfo>			stages;						// 044
	TList<LocalVariableOrObjectivePtr>	lVarOrObjectives;	// 04C	So: this list would contain both Objectives and LocalVariables !
		// That seems very strange but still, looking at Get/SetObjective... and ShowQuestVars there's no doubt.
	TList<Condition*>			conditions;					// 054
	ScriptEventList*			scriptEventList;			// 05C
	UInt8						currentStage;				// 060
	UInt8						pad061[3];					// 061
	String						editorName;					// 064

	BGSQuestObjective* GetObjective(UInt32 objectiveID) const;
};
static_assert(sizeof(TESQuest) == 0x6C);

// TESIdleForm (54)
class TESIdleForm;

class TESPackageData
{
public:
	TESPackageData();
	~TESPackageData();
	virtual void	Destroy(bool free);
	virtual void	CopyFrom(TESPackageData * packageData);
	virtual void	Unk_02();
	virtual void	Save(ModInfo* modInfo);
	virtual void	Unk_04();
	virtual void	Unk_05();
	virtual void	Unk_06();
	virtual void	Unk_07();
};

class TESPatrolPackageData : public TESPackageData
{
public:
	TESPatrolPackageData();
	~TESPatrolPackageData();

	UInt8	patrolFlags;
};

// TESPackage (80) class definition duplicated from Oblivion. Enum not updated
class TESPackage : public TESForm
{
public:
	TESPackage();
	~TESPackage();

	enum ePackageFlag	// From OBSE and FNVEdit
	{
		kPackageFlag_OffersServices =			1 << 0,
		kPackageFlag_MustReachLocation =		1 << 1,
		kPackageFlag_MustComplete =				1 << 2,
		kPackageFlag_LockDoorsAtStart =			1 << 3,
		kPackageFlag_LockDoorsAtEnd =			1 << 4, 	// set by CHANGE_PACKAGE_WAITING ?
		kPackageFlag_LockDoorsAtLocation =		1 << 5,
		kPackageFlag_UnlockDoorsAtStart =		1 << 6,
		kPackageFlag_UnlockDoorsAtEnd =			1 << 7,
		kPackageFlag_UnlockDoorsAtLocation =	1 << 8,
		kPackageFlag_ContinueIfPCNear =			1 << 9,
		kPackageFlag_OncePerDay =				1 << 10,
		kPackageFlag_Unk11 =					1 << 11,
		kPackageFlag_SkipFalloutBehavior =		1 << 12,
		kPackageFlag_AlwaysRun =				1 << 13,
		kPackageFlag_Unk14 =					1 << 14,
		kPackageFlag_NeverRun =					1 << 15,	// Save only ?
		kPackageFlag_Unk16 =					1 << 16,
		kPackageFlag_AlwaysSneak =				1 << 17,
		kPackageFlag_AllowSwimming =			1 << 18,
		kPackageFlag_AllowFalls =				1 << 19,
		kPackageFlag_ArmorUnequipped =			1 << 20,
		kPackageFlag_WeaponsUnequipped =		1 << 21,
		kPackageFlag_DefensiveCombat =			1 << 22,
		kPackageFlag_WeaponsDrawn =				1 << 23,
		kPackageFlag_NoIdleAnims =				1 << 24,
		kPackageFlag_PretendInCombat =			1 << 25,
		kPackageFlag_ContinueDuringCombat =		1 << 26,
		kPackageFlag_NoCombatAlert =			1 << 27,
		kPackageFlag_NoWarnAttackBehavior =		1 << 28,
		kPackageFlag_Unk29 =					1 << 29,
		kPackageFlag_Unk30 =					1 << 30,
		kPackageFlag_Unk31 =					1 << 31
	};

	enum ePackageType	// From OBSE and FNVEdit. Runtimes has 0x24 types!
	{
		kPackageType_Min		= 0,
		kPackageType_Find		= 0,		// 00
		kPackageType_Follow,
		kPackageType_Escort,
		kPackageType_Eat,
		kPackageType_Sleep,
		kPackageType_Wander,
		kPackageType_Travel,
		kPackageType_Accompany,
		kPackageType_UseItemAt,
		kPackageType_Ambush,
		kPackageType_FleeNotCombat,
		kPackageType_Unk11,
		kPackageType_Sandbox,
		kPackageType_Patrol,
		kPackageType_Guard,
		kPackageType_Dialogue,
		kPackageType_UseWeapon,			// 10

		// unless shown otherwise kPackageType_CombatController,	// Actor::GetCombatController returns package only if type matches this
		// start conversation can lead to a package of type 1C, which is recorded in PlayerCharacter::Unk0224

		kPackageType_Max
	};

	// 8
	struct PackageTime
	{
		enum
		{
			kDay_Any = 0,
			kTime_Any = 0xFF,
		};

		enum
		{
			kMonth_January = 0,
			kMonth_February,
			kMonth_March,
			kMonth_April,
			kMonth_May,
			kMonth_June,
			kMonth_July,
			kMonth_August,
			kMonth_September,
			kMonth_October,
			kMonth_November,
			kMonth_December,
			kMonth_Spring,	// march, april, may
			kMonth_Summer,	// june, july, august
			kMonth_Autumn,	// september, august, november (in Geck)
			kMonth_Winter,	// december, january, february

			kMonth_Any = 0xFF,
		};

		enum
		{
			kWeekday_Sundays = 0,
			kWeekday_Morndays,
			kWeekday_Tuesdays,
			kWeekday_Wednesdays,
			kWeekday_Thursdays,
			kWeekday_Frydays,
			kWeekday_Saturdays,
			kWeekday_Weekdays,
			kWeekday_Weekends,
			kWeekday_MWF,
			kWeekday_TT,

			kWeekday_Any = 0xFF
		};

		UInt8	month;
		UInt8	weekDay;
		UInt8	date;
		UInt8	time;
		UInt32	duration;

		static const char* MonthForCode(UInt8 monthCode);
		static const char* DayForCode(UInt8 dayCode);
		static UInt8 CodeForMonth(const char* monthStr);
		static UInt8 CodeForDay(const char* dayStr);
		static bool IsValidMonth(UInt8 m) { return (m+1) <= kMonth_Winter; }
		static bool IsValidTime(UInt8 t) { return (t+1) <= 24; }
		static bool IsValidDay(UInt8 d) { return (d+1) <= kWeekday_TT; }
		static bool IsValidDate(UInt8 d) { return d <= 31; }
	};

	// order only somewhat related to kFormType_XXX (values off by 17, 20, or 21)
	enum eObjectType// From OBSE and FNVEdit
	{
		kObjectType_Min		= 0,
		kObjectType_None	= 0,
		kObjectType_Activators,
		kObjectType_Armor,
		kObjectType_Books,
		kObjectType_Clothing,
		kObjectType_Containers,
		kObjectType_Doors,
		kObjectType_Ingredients,
		kObjectType_Lights,
		kObjectType_Misc,
		kObjectType_Flora,
		kObjectType_Furniture,
		kObjectType_WeaponsAny,
		kObjectType_Ammo,
		kObjectType_NPCs,
		kObjectType_Creatures,
		kObjectType_Keys,				//	10
		kObjectType_Alchemy,
		kObjectType_Food,
		kObjectType_AllCombatWearable,
		kObjectType_AllWearable,
		kObjectType_WeaponsRanged,
		kObjectType_WeaponsMelee,
		kObjectType_WeaponsNone,
		kObjectType_ActorEffectAny,
		kObjectType_ActorEffectRangeTarget,
		kObjectType_ActorEffectRangeTouch,
		kObjectType_ActorEffectRangeSelf,
		kObjectType_ActorsAny,

		kObjectType_Max,						//	1E
	};

	union ObjectType
	{
		TESForm*		form;
		TESObjectREFR*	refr;
		eObjectType		objectCode;
	};


	struct LocationData
	{
		enum ePackageLocation {
			kPackLocation_Min				= 0,
			kPackLocation_NearReference		= 0,
			kPackLocation_InCell			= 1,
			kPackLocation_CurrentLocation	= 2,
			kPackLocation_EditorLocation	= 3,
			kPackLocation_ObjectID			= 4,
			kPackLocation_ObjectType		= 5,
			kPackLocation_LinkedReference	= 6,

			kPackLocation_Max,
		};

		UInt8		locationType;	// 000
		UInt8		pad[3];
		UInt32		radius;			// 004
		ObjectType  object;			// 008

		static LocationData*	Create();
		static const char*		StringForLocationCode(ePackageLocation locCode);
		const char*				StringForLocationCodeAndData();
		static UInt8			LocationCodeForString(const char* locStr);
		static bool				IsValidLocationType(UInt8 locCode) { return locCode < kPackLocation_Max; }
	};

	enum
	{
		kTargetType_Refr		= 0,
		kTargetType_BaseObject	= 1,
		kTargetType_TypeCode	= 2,
		
		kTargetType_Max	= 3,
	};

	struct TargetData 
	{
		UInt8		targetType;	// 00
		UInt8		pad[3];		// 01
		ObjectType	target;		// 04
		UInt32		count;		// 08 can be distance too
		float		unk0C;		// 0C

		static TargetData*	Create();
		static const char*	StringForTargetCode(UInt8 targetCode);
		const char*			StringForTargetCodeAndData();
		static UInt8		TargetCodeForString(const char* targetStr);
		static bool			IsValidTargetCode(UInt8 c) { return c < TESPackage::kTargetType_Max; }
	};


	enum eProcedure {			// UInt32	// Checked the Geck Wiki. Not consistent with s_procNames (which has a diffferent order and 0x37 procedures)
		kProcedure_Min		= 0,
		kProcedure_TRAVEL	= 0,
		kProcedure_ACTIVATE,
		kProcedure_ACQUIRE,
		kProcedure_WAIT,
		kProcedure_DIALOGUE,
		kProcedure_GREET,
		kProcedure_GREET_DEAD,
		kProcedure_WANDER,
		kProcedure_SLEEP,
		kProcedure_OBSERVE_COMBAT,
		kProcedure_EAT,
		kProcedure_FOLLOW,
		kProcedure_ESCORT,
		kProcedure_COMBAT,
		kProcedure_ALARM,
		kProcedure_PURSUE,
		kProcedure_FLEE,					// 0x10
		kProcedure_DONE,
		kProcedure_YELD,
		kProcedure_TRAVEL_TARGET,
		kProcedure_CREATE_FOLLOW,
		kProcedure_GET_UP,
		kProcedure_MOUNT_HORSE,
		kProcedure_DISMOUNT_HORSE,
		kProcedure_DO_NOTHING,
		kProcedure_UNK019,
		kProcedure_UNK01A,
		kProcedure_ACCOMPANY,
		kProcedure_USE_ITEM_AT,
		kProcedure_AIM,
		kProcedure_NOTIFY,
		kProcedure_SANDMAN,
		kProcedure_WAIT_AMBUSH,
		kProcedure_SURFACE,					// 0x20
		kProcedure_WAIT_FOR_SPELL,
		kProcedure_CHOOSE_CAST,
		kProcedure_FLEE_NON_COMBAT,
		kProcedure_REMOVE_WORN_ITEMS,
		kProcedure_SEARCH,
		kProcedure_CLEAR_MOUNT_POSITION,
		kProcedure_SUMMON_CREATURE_DEFEND,
		kProcedure_AVOID_AREA,
		kProcedure_UNEQUIP_ARMOR,
		kProcedure_PATROL,
		kProcedure_USE_WEAPON,
		kProcedure_DIALOGUE_ACTIVATE,
		kProcedure_GUARD,
		kProcedure_SANDBOX,
		kProcedure_USE_IDLE_MARKER,
		kProcedure_TAKE_BACK_ITEM,
		kProcedure_SITTING,					// 0x30
		kProcedure_MOVEMENT_BLOCKED,
		kProcedure_CANIBAL_FEED,			// 0x32

		kProcedure_Max						// 0x33
	};

	// In DialoguePackage, there are 0x58 virtual functions (including 0x4E from TESForm)

	UInt32				procedureArrayIndex;	// 018 index into array of array of eProcedure terminated by 0x2C. 
												//	   -1 if no procedure array exists for package type.
	UInt32				packageFlags;			// 01C
	UInt8				type;					// 020
	UInt8				pad021[1];				// 021
	UInt16				behaviorFlags;			// O22
	UInt32				specificFlags;			// 024
	TESPackageData*		packageData;			// 028
	LocationData*		location;				// 02C
	TargetData*			target;					// 030	target ?
	UInt32				unk034;					// 034	idles
	PackageTime			time;					// 038
	UInt32				unk040[(0x80 - 0x40) >> 2];
		// 	040 is a TList of Condition, 7C is an Interlocked counter
		//	048 is a DWord CombatStyle, 
		//	04C, 05C and 06C are the same 4 DWord struct onBegin onEnd onChange, { TESIdle* idle; EmbeddedScript* script; Topic* topic; UInt32 unk0C; }
		//	07C is a DWord

	void				SetTarget(TESObjectREFR* refr);
	void				SetTarget(TESForm* baseForm, UInt32 count);
	void				SetTarget(eObjectType typeCode, UInt32 count);
	void				SetCount(UInt32 aCount);
	void				SetDistance(UInt32 aDistance) { SetCount(aDistance); }
	TargetData*			GetTargetData();
	LocationData*		GetLocationData();

	bool				IsFlagSet(UInt32 flag);
	void				SetFlag(UInt32 flag, bool bSet);

	static const char*	StringForPackageType(ePackageType pkgType);
	static const char*	StringForObjectCode(eObjectType objCode);
	static UInt8		ObjectCodeForString(const char* objString);
	static bool			IsValidObjectCode(UInt8 o) { return o < kObjectType_Max; }
	static const char*	StringForProcedureCode(eProcedure proc);
	static const char*	StringForProcedureCode(eProcedure proc, bool bRemovePrefix);
};

static_assert(sizeof(TESPackage) == 0x80);

struct DialogueResponse {
	String			responseText;	// 000
	UInt32			emotionType;	// 008
	SInt32			emotionValue;	// 00C
	String			voiceFileName;	// 010
	TESIdleForm*	speakerIdle;	// 018	idle
	TESIdleForm*	listenerIdle;	// 01C	idle
	TESSound*		sound;			// 020
	UInt32			unk024;			// 024	uninitialized, unused in xEdit
	UInt32			unk028;			// 028
};

struct DialogueItem {
	TList<DialogueResponse>	responses;				// 000
	DialogueResponse*	currentResponse;	// 008
	TESTopicInfo*		currentTopicInfo;	// 00C
	TESTopic*			currentTopic;		// 010
	TESQuest*			currentQuest;		// 014
	TESForm*			currentSpeaker;		// 018
};

// DialoguePackage : Only package tested and verified effectivly
class DialoguePackage : public TESPackage
{
public:
	DialoguePackage();
	~DialoguePackage();

	struct Data0080 {	// The constructor is referenced 169 time all over the place !
		UInt32	unk000;		//	000
		UInt8	byt004;		//	004
		UInt8	pad005[3];	//	005-007
		UInt32	unk008;		//	008
	};	//	00C

	Data0080	data0080;		// 080
	TESTopic*	topic;			// 08C
	float		flt090;			// 090
	Character*	speaker;		// 094
	UInt8		unk098;			// 098
	UInt8		unk099;			// 098
	UInt8		unk09A;			// 098
	UInt8		unk09B;			// 098
	TESForm*	unk09C;			// 09C
	UInt32		unk0A0;			// 0A0
	void*		unk0A4;			// 0A4	list of Dialogue Item and Dialogue Response, plus current item and current response
	UInt32		unk0A8;			// 0A8
	UInt32		unk0AC;			// 0AC
	Character*	subject;		// 0B0
	Character*	target;			// 0B4
	TESForm*	unk0B8;			// 0B8
	UInt8		unk0BC;			// 0BC
	UInt8		unk0BD;			// 0BD
	UInt8		unk0BE;			// 0BE
	UInt8		unk0BF;			// 0BF
	UInt8		unk0C0;			// 0C0
	UInt8		unk0C1;			// 0C1
	UInt8		unk0C2;			// 0C2
	UInt8		unk0C3;			// 0C3
	UInt32		unk0C4;			// 0C4
	UInt32		unk0C8;			// 0C8
	UInt8		unk0CC;			// OCC
	UInt8		unk0CD[3];		// OCD
};	// 0D0

typedef struct {
	float vector[3];
} ThreeFloatArray;

class FleePackage : public TESPackage
{
public:
	FleePackage();
	~FleePackage();

	UInt8			unk080;		// 080
	UInt8			unk081;		// 081
	UInt8			pad082[2];	// 082
	ThreeFloatArray	unk084;		// 084	is array of 3 floats, should be Pos
	float			unk090;		// 090
	UInt8			unk094;		// 094
	UInt8			pad095[3];	// 095
	TList<TESForm*>	list098;	// 098
	TESForm*		unk0A0;		// 0A0
	TESForm*		unk0A4;		// 0A4
	UInt8			unk0A8;		// 0A8
	UInt8			unk0A9;		// 0A9
	UInt8			pad0AA[2];	// 0AA
};	// 0AC

class TressPassPackage : public TESPackage
{
public:
	TressPassPackage();
	~TressPassPackage();

	float		unk080;		// 080
	UInt32		unk084;		// 084
	TESForm*	unk088;		// 088
	TESForm*	unk08C;		// 08C
	UInt32		unk090;		// 090
	UInt32		unk094;		// 094
	UInt32		unk098;		// 098
};	// 09C

struct SpectatorThreatInfo
{
	TESForm*		unk000;		// 000
	TESForm*		unk004;		// 004
	UInt32			unk008;		// 008
	UInt32			unk00C;		// 00C	elapsed tick count
	UInt32			unk010;		// 010
	ThreeFloatArray	unk014;		// 014	is array of 3 floats, should be Pos
	ThreeFloatArray	unk020;		// 020	is array of 3 floats, should be Rot
	UInt8			unk02C;		// 02C
	UInt8			unk02D;		// 02D
	UInt8			pad[2];		// 02E
};	// 030

class SpectatorPackage : public TESPackage
{
public:
	SpectatorPackage();
	~SpectatorPackage();

	UInt32			unk080;		// 080
	UInt32			unk084;		// 084
	UInt32			unk088;		// 088
	UInt32			unk08C;		// 08C
	UInt8			unk090;		// 090
	UInt8			pad091[3];	// 091
	ThreeFloatArray	unk094;		// 094	is array of 3 floats, should be Pos
	BSSimpleArray<SpectatorThreatInfo>	arr0A0;	// 0A0
	// There is an object containing a semaphore at B0/B4
};	// 0B4

class TESFollowPackageData : public TESPackageData
{
public:
	TESFollowPackageData();
	~TESFollowPackageData();
	TESPackage::LocationData* endLocation;
	float	flt008;
};

// TESCombatStyle (108)
class TESCombatStyle : public TESForm
{
public:
	TESCombatStyle();
	~TESCombatStyle();

	enum
	{
		kFlag_ChooseAttackUsingChance		= 1 << 0,
		kFlag_MeleeAlertOK					= 1 << 1,
		kFlag_FleeBasedOnPersonalSurvival	= 1 << 2,

		kFlag_IgnoreThreats					= 1 << 4,
		kFlag_IgnoreDamagingSelf			= 1 << 5,
		kFlag_IgnoreDamagingGroup			= 1 << 6,
		kFlag_IgnoreDamagingSpectators		= 1 << 7,
		kFlag_CannotUseStealthboy			= 1 << 8,
	};

	float	coverSearchRadius;				// 018
	float	takeCoverChance;				// 01C
	float	waitTimeMin;					// 020
	float	waitTimeMax;					// 024
	float	waitToFireTimerMin;				// 028
	float	waitToFireTimerMax;				// 02C
	float	fireTimerMin;					// 030
	float	fireTimerMax;					// 034
	float	rangedWeapRangeMultMin;			// 038
	UInt8	pad3C[4];						// 03C
	UInt8	weaponRestrictions;				// 040
	UInt8	pad41[3];						// 041
	float	rangedWeapRangeMultMax;			// 044
	float	maxTargetingFOV;				// 048
	float	combatRadius;					// 04C
	float	semiAutoFiringDelayMultMin;		// 050
	float	semiAutoFiringDelayMultMax;		// 054
	UInt8	dodgeChance;					// 058
	UInt8	LRChance;						// 059
	UInt8	pad5A[2];						// 05A
	float	dodgeLRTimerMin;				// 05C
	float	dodgeLRTimerMax;				// 060
	float	dodgeFWTimerMin;				// 064
	float	dodgeFWTimerMax;				// 068
	float	dodgeBKTimerMin;				// 06C
	float	dodgeBKTimerMax;				// 070
	float	idleTimerMin;					// 074
	float	idleTimerMax;					// 078
	UInt8	blockChance;					// 07C
	UInt8	attackChance;					// 07D
	UInt8	pad7E[2];						// 07E
	float	staggerBonusToAttack;			// 080
	float	KOBonusToAttack;				// 084
	float	H2HBonusToAttack;				// 088
	UInt8	powerAttackChance;				// 08C
	UInt8	pad8D[3];						// 08D
	float	staggerBonusToPower;			// 090
	float	KOBonusToPower;					// 094
	UInt8	powerAttackN;					// 098
	UInt8	powerAttackF;					// 099
	UInt8	powerAttackB;					// 09A
	UInt8	powerAttackL;					// 09B
	UInt8	powerAttackR;					// 09C
	UInt8	pad9D[3];						// 09D
	float	holdTimerMin;					// 0A0
	float	holdTimerMax;					// 0A4
	UInt16	csFlags;						// 0A8
	UInt8	pad10A[2];						// 0AA
	UInt8	acrobaticDodgeChance;			// 0AC
	UInt8	rushAttackChance;				// 0AD
	UInt8	pad10E[2];						// 0AE
	float	rushAttackDistMult;				// 0B0
	float	dodgeFatigueModMult;			// 0B4
	float	dodgeFatigueModBase;			// 0B8
	float	encumSpeedModBase;				// 0BC
	float	encumSpeedModMult;				// 0C0
	float	dodgeUnderAttackMult;			// 0C4
	float	dodgeNotUnderAttackMult;		// 0C8
	float	dodgeBackUnderAttackMult;		// 0CC
	float	dodgeBackNotUnderAttackMult;	// 0D0
	float	dodgeFWAttackingMult;			// 0D4
	float	dodgeFWNotAttackingMult;		// 0D8
	float	blockSkillModMult;				// 0DC
	float	blockSkillModBase;				// 0E0
	float	blockUnderAttackMult;			// 0E4
	float	blockNotUnderAttackMult;		// 0E8
	float	attackSkillModMult;				// 0EC
	float	attackSkillModBase;				// 0F0
	float	attackUnderAttackMult;			// 0F4
	float	attackNotUnderAttackMult;		// 0F8
	float	attackDuringBlockMult;			// 0FC
	float	powerAttackFatigueModBase;		// 100
	float	powerAttackFatigueModMult;		// 104

	void SetFlag(UInt32 pFlag, bool bMod) { csFlags = bMod ? (csFlags | pFlag) : (csFlags & ~pFlag); }
};
static_assert(sizeof(TESCombatStyle) == 0x108);

// 2C
class TESRecipeCategory : public TESForm
{
public:
	TESRecipeCategory();
	~TESRecipeCategory();

	TESFullName			fullName;	// 18
	UInt32				flags;		// 24
};
static_assert(sizeof(TESRecipeCategory) == 0x28);

struct RecipeComponent
{
	UInt32		quantity;
	TESForm* item;
};

// 5C
class TESRecipe : public TESForm
{
public:
	TESRecipe();
	~TESRecipe();

	TESFullName				fullName;		// 18

	UInt32					reqSkill;		// 24
	UInt32					reqSkillLevel;	// 28
	UInt32					categoryID;		// 2C
	UInt32					subCategoryID;	// 30
	TList<Condition>		conditions;		// 34
	TList<RecipeComponent>	inputs;			// 3C
	TList<RecipeComponent>	outputs;		// 44
	UInt32					unk4C;			// 4C
	UInt32					unk50;			// 50
	TESRecipeCategory*		category;		// 54
	TESRecipeCategory*		subCategory;	// 58
};
static_assert(sizeof(TESRecipe) == 0x5C);

// TESLoadScreen (3C)
class TESLoadScreen : public TESForm
{
public:
	TESLoadScreen();
	~TESLoadScreen();

	TESTexture		texture;			// 018
	TESDescription	description;		// 024
	UInt32	unk2C[(0x3C - 0x2C) >> 2];	// 02C
};

// TESLevSpell (44)
class TESLevSpell;

// TESObjectANIO (3C)
class TESObjectANIO : public TESForm
{
public:
	TESObjectANIO();
	~TESObjectANIO();

	TESModelTextureSwap	modelSwap;		// 18
	UInt32				unk38;			// 38
};

// TESWaterForm (190, 194 in 1.5)
class TESWaterForm : public TESForm
{
public:
	TESWaterForm();
	~TESWaterForm();

	TESFullName				fullName;		// 018
	TESAttackDamageForm		attackDamage;	// 024

	UInt32 unk2C[(0x190 - 0x2C) >> 2];		// 02C

	// 080 has a TESWaterForm * added in patch 1.5
};

// TESEffectShader (170)
class TESEffectShader : public TESForm
{
public:
	TESEffectShader();
	~TESEffectShader();

	UInt32 unk018[(0x170 - 0x18) >> 2];
};


/**** BSTempEffects ****/

// 18
class BSTempEffect : public NiObject
{
public:
	BSTempEffect();
	~BSTempEffect();

	float			duration;		// 08
	TESObjectCELL* cell;			// 0C
	float			unk10;			// 10
	UInt8			unk14;			// 14
	UInt8			pad15[3];
};

// 28
class MagicHitEffect : public BSTempEffect
{
public:
	MagicHitEffect();
	~MagicHitEffect();

	ActiveEffect* activeEffect;	// 18	
	TESObjectREFR* target;		// 1C
	float			unk20;			// 20	Init'd from ActiveEffect.timeElapsed
	UInt8			unk24;			// 24	from ActiveEffect.EffectFlag
	UInt8			pad25[3];
};

class ParticleShaderProperty;

// 6C
class MagicShaderHitEffect : public MagicHitEffect
{
public:
	MagicShaderHitEffect();
	~MagicShaderHitEffect();

	UInt8					unk28;						// 28	Init'd to byte, OK for first offset.
	UInt8					pad29[3];
	UInt32					unk2C;						// 2C	Init'd to DWord
	TESEffectShader* effectShader;				// 30	Init'd to *effectShader
	float					unk34;						// 34	Init'd to float
	BSSimpleArray<NiPointer<ParticleShaderProperty>>	unk38;	// 38	Init'd to BSSimpleArray<NiPointer<ParticleShaderProperty>>
	// the remainder is not validated..
	void* textureEffectData;		// 48 seen TextureEffectData< BSSahderLightingProperty >, init'd to RefNiObject
};	// Alloc'd to 6C, 68 is RefNiObject, 60 is Init'd to 1.0, 64 also
	// 4C is byte, Init'd to 0 for non player, otherwize = Player.1stPersonSkeleton.Flags0030.Bit0 is null

// BGSDebris (24)
class BGSDebris : public TESForm
{
	BGSDebris();
	~BGSDebris();

	BGSPreloadable				preloadable;	// 018
	UInt32	unk01C;
	UInt32	unk020;
};

// TESImageSpace (B0)
class TESImageSpace : public TESForm
{
public:
	TESImageSpace();
	~TESImageSpace();

	UInt32 unk018[(0xB0-0x18) >> 2];		// 018
};
static_assert(sizeof(TESImageSpace) == 0xB0);

// TESImageSpaceModifier (728)
class TESImageSpaceModifier : public TESForm
{
public:
	TESImageSpaceModifier();
	~TESImageSpaceModifier();

	UInt32 unk018[(0x728-0x18) >> 2];		// 018
};
static_assert(sizeof(TESImageSpaceModifier) == 0x728);

// 24
class BGSListForm : public TESForm
{
public:
	BGSListForm();
	~BGSListForm();

	TList<TESForm> list;			// 018

	UInt32	numAddedObjects;	// number of objects added via script - assumed to be at the start of the list

	UInt32 Count() const { return list.count(); }

	TESForm* GetNthForm(const SInt32 n) const { return list.GetNth(n); }

	UInt32 AddAt(TESForm* pForm, const SInt32 n) {
		const SInt32 result = list.AddAt(pForm, n);

		if (result >= 0 && IsAddedObject(n)) numAddedObjects++;

		return result;
	}

	SInt32 GetIndexOf(TESForm* pForm);

	TESForm* RemoveNthForm(SInt32 n) {
		TESForm* form = list.RemoveNth(n);

		if (form && IsAddedObject(n))
			if (numAddedObjects == 0)
				Log() << ("BGSListForm::RemoveNthForm: numAddedObjects = 0");
			else
				numAddedObjects--;

		return form;
	}

	TESForm* ReplaceNthForm(SInt32 n, TESForm* pReplaceWith) {
		return list.ReplaceNth(n, pReplaceWith);
	}

	SInt32 RemoveForm(TESForm* pForm);
	SInt32 ReplaceForm(TESForm* pForm, TESForm* pReplaceWith);

	bool IsAddedObject(SInt32 idx)
	{
		return (idx >= 0) && (idx < numAddedObjects);
	}

	bool Contains(TESForm* form);
	bool ContainsRecursive(TESForm* form, UInt32 reclvl = 0);
};
static_assert(sizeof(BGSListForm) == 0x024);

class BGSPerkEntry
{
public:
	BGSPerkEntry();
	~BGSPerkEntry();

	virtual void CheckConditionFilters();
	virtual void GetPriority();
	virtual void GetData();
	virtual void Fn_03();
	virtual UInt32 GetType();
	virtual void Fn_06();
	virtual void Fn_07();
	virtual void Fn_08();
	virtual void Fn_09();
	virtual void Fn_0A();
	virtual void Fn_0B();
	virtual void Fn_0C();
	virtual void Fn_0D();

	UInt8				rank;				// 04 +1 for value shown in GECK
	UInt8				priority;			// 05
	UInt16				type;				// 06 (Quest: 0xC24, Ability: 0xB27, Entry Point: 0xD16)
};
static_assert(sizeof(BGSPerkEntry) == 0x8);

class BGSQuestPerkEntry : public BGSPerkEntry
{
public:
	BGSQuestPerkEntry();
	~BGSQuestPerkEntry();

	TESQuest*			quest;				// 08
	UInt8				stage;				// 0C
	UInt8				pad[3];				// 0D
};
static_assert(sizeof(BGSQuestPerkEntry) == 0x10);

class BGSAbilityPerkEntry : public BGSPerkEntry
{
public:
	BGSAbilityPerkEntry();
	~BGSAbilityPerkEntry();

	SpellItem*			ability;			// 08
};
static_assert(sizeof(BGSAbilityPerkEntry) == 0xC);

class BGSEntryPointFunctionData
{
public:
	BGSEntryPointFunctionData();
	~BGSEntryPointFunctionData();

	UInt32				vtbl;				// 00
};
static_assert(sizeof(BGSEntryPointFunctionData) == 0x4);

// 08
class BGSEntryPointFunctionDataOneValue : public BGSEntryPointFunctionData
{
public:
	BGSEntryPointFunctionDataOneValue();
	~BGSEntryPointFunctionDataOneValue();

	Float32				value;				// 04
};
static_assert(sizeof(BGSEntryPointFunctionDataOneValue) == 0x8);

// 0C
class BGSEntryPointFunctionDataTwoValue : public BGSEntryPointFunctionData
{
public:
	BGSEntryPointFunctionDataTwoValue();
	~BGSEntryPointFunctionDataTwoValue();

	Float32				value[2];			// 04
};
static_assert(sizeof(BGSEntryPointFunctionDataTwoValue) == 0xC);

class BGSEntryPointFunctionDataLeveledList : public BGSEntryPointFunctionData
{
public:
	BGSEntryPointFunctionDataLeveledList();
	~BGSEntryPointFunctionDataLeveledList();

	TESLevItem			*leveledList;		// 04
};
static_assert(sizeof(BGSEntryPointFunctionDataLeveledList) == 0x8);

class BGSEntryPointFunctionDataActivateChoice : public BGSEntryPointFunctionData
{
public:
	BGSEntryPointFunctionDataActivateChoice();
	~BGSEntryPointFunctionDataActivateChoice();

	String					label;			// 04
	Script*					script;			// 0C
	UInt32					flags;			// 10
};
static_assert(sizeof(BGSEntryPointFunctionDataActivateChoice) == 0x14);

class BGSEntryPointPerkEntry : public BGSPerkEntry
{
public:
	BGSEntryPointPerkEntry();
	~BGSEntryPointPerkEntry();

	struct EntryPointConditions
	{
		TList<Condition>		tab1;
		TList<Condition>		tab2;
		TList<Condition>		tab3;
	};

	UInt8						entryPoint;		// 08
	UInt8						function;		// 09
	UInt8						conditionTabs;	// 0A
	UInt8						pad0B;			// 0B
	BGSEntryPointFunctionData*	data;			// 0C
	EntryPointConditions*		conditions;		// 10
};
static_assert(sizeof(BGSEntryPointPerkEntry) == 0x14);

// 50
class BGSPerk : public TESForm
{
public:
	BGSPerk();
	~BGSPerk();

	struct PerkData
	{
		UInt8				isTrait;	// 00
		UInt8				minLevel;	// 01
		UInt8				numRanks;	// 02
		UInt8				isPlayable;	// 03 unverified
		UInt8				isHidden;	// 04 unverified
		UInt8				unk05;		// 05 
		UInt8				unk06;		// 06
		UInt8				unk07;		// 07
	};

	TESFullName				fullName;			// 18
	TESDescription			description;		// 24
	TESIcon					icon;				// 2C
	PerkData				data;				// 38
	TList<Condition>		conditions;			// 40
	TList<BGSPerkEntry>		entries;			// 48
};
static_assert(sizeof(BGSPerk) == 0x50);

// 74
class BGSBodyPartData : public TESForm
{
public:
	BGSBodyPartData();
	~BGSBodyPartData();

	enum
	{
		eBodyPart_Torso = 0,
		eBodyPart_Head1,
		eBodyPart_Head2,
		eBodyPart_LeftArm1,
		eBodyPart_LeftArm2,
		eBodyPart_RightArm1,
		eBodyPart_RightArm2,
		eBodyPart_LeftLeg1,
		eBodyPart_LeftLeg2,
		eBodyPart_LeftLeg3,
		eBodyPart_RightLeg1,
		eBodyPart_RightLeg2,
		eBodyPart_RightLeg3,
		eBodyPart_Brain,
		eBodyPart_Weapon,
	};

	TESModel		model;				// 018
	BGSPreloadable	preloadable;		// 030
	BGSBodyPart*	bodyParts[15];		// 034
	BGSRagdoll*		ragDoll;			// 070
};
static_assert(sizeof(BGSBodyPartData) == 0x74);

// ActorValueInfo (C4)
class ActorValueInfo : public TESForm {
public:
	UInt32		fill_018[(0x038-0x018) >> 2];			// 018 0037
	char*		actorValueName;							// 038
	UInt32		fill_03C[(0x044-0x03C) >> 2];			// 03C 043
	UInt32		avFlags;								// 044
		//		bit 0x01	used in list of modified ActorValue for Player and others. Either can damage or "special damage", see 0x00937280
		//		bit 0x03
		//		bit 0x04
		//		bit 0x07
		//		bit 0x08
		//		bit 0x0B
		//		bit 0x0C
		//		bit 0x0E	canModify
	UInt32		unk_048;								// 048
	UInt32		unk_04C;								// 04C		// Address of callback used in GetValueInfo, result in st
	UInt32		unk_050;								// 050
	UInt32		unk_054;								// 054		// Address of a callback used in SetActorValue
	UInt32		fill_058[(0x098-0x058) >> 2];			// 058 097
	UInt32		unk_098;								// 098
	UInt32		fill_09C[(0x0C4-0x09C) >> 2];			// 09C 0C3
};
static_assert(sizeof(ActorValueInfo) == 0x0C4);

extern const ActorValueInfo** ActorValueInfoPointerArray;

typedef ActorValueInfo* (* _GetActorValueInfo)(UInt32 actorValueCode);
extern const _GetActorValueInfo GetActorValueInfo;

class BGSRadiationStage : public TESForm
{
public:
	BGSRadiationStage();
	~BGSRadiationStage();

	UInt32	unk018;			// 018
	UInt32	unk01C;			// 01C
};
static_assert(sizeof(BGSRadiationStage) == 0x20);

// BGSCameraShot (78)
class BGSCameraShot : public TESForm
{
	BGSCameraShot();
	~BGSCameraShot();

//	TESModel	model;								// 01C
//	TESImageSpaceModifiableForm imageSpaceModForm;	// 020

	UInt32 unk024[(0x78-0x18) >> 2];	// 024
};
static_assert(sizeof(BGSCameraShot) == 0x78);

// BGSCameraPath (38)
class BGSCameraPath;

class BGSVoiceType : public TESForm
{
public:
	BGSVoiceType();
	~BGSVoiceType();

	UInt32		unk018;		// 018
	UInt32		unk01C;		// 01C
	UInt32		unk020;		// 020
};
static_assert(sizeof(BGSVoiceType) == 0x24);

struct ColorRGB
{
	UInt8	red;	// 000
	UInt8	green;	// 001
	UInt8	blue;	// 002
	UInt8	alpha;	// 003 or unused if no alpha
};	// 004 looks to be endian swapped !

struct DecalData
{
	float		minWidth;		// 000
	float		maxWidth;		// 004
	float		minHeight;		// 008
	float		maxHeight;		// 00C
	float		depth;			// 010
	float		shininess;		// 014
	float		parallaxScale;	// 018
	UInt8		parallaxPasses;	// 01C
	UInt8		flags;			// 01D	Parallax, Alpha - Blending, Alpha - Testing
	UInt8		unk01E[2];		// 01E
	ColorRGB	color;			// 020
};
static_assert(sizeof(DecalData) == 0x024);

class BGSImpactData : public TESForm
{
public:
	BGSImpactData();
	~BGSImpactData();

	struct Data
	{
		Float32		effectDuration;		// 000
		UInt32		effectorientation;	// 004	Surface Normal, Projectile vector, Projectile reflection
		Float32		angleThreshold;		// 008
		Float32		placementRadius;	// 00C
		UInt32		soundLevel;			// 010	enum
		UInt32		flags;				// 014	No decal data
	};	// 018

	TESModel		model;				// 018
	Data			data;				// 030	DATA
	TESTexture*		textureSet;			// 048 DNAM Texture Set
	TESSound*		sound1;				// 04C
	TESSound*		sound2;				// 050
	DecalData		decalData;			// 054 DODT [begining of DATA before form version 0x0A]
};
static_assert(sizeof(BGSImpactData) == 0x078);

// 4C
class BGSImpactDataSet : public TESForm
{
public:
	BGSImpactDataSet();
	~BGSImpactDataSet();

	BGSPreloadable	preloadable;				// 018
	UInt32			impactDatas[12];			// 01C
};
static_assert(sizeof(BGSImpactDataSet) == 0x4C);

class BGSEncounterZone : public TESForm
{
	BGSEncounterZone();
	~BGSEncounterZone();

	UInt32 unk018[(0x30-0x18) >> 2];	// 018
};
static_assert(sizeof(BGSEncounterZone) == 0x30);

class BGSMessage : public TESForm
{
public:
	BGSMessage();
	~BGSMessage();

	struct Button {
		String				buttonText;		// 000
		TList<Condition>	condition;		// 008
	};

	TESFullName			fullName;			// 018
	TESDescription		description;		// 024

	BGSMenuIcon*		menuIcon;			// 2C
	TList<Button>		buttons;			// 030
	UInt32				messageFlags;		// 038 init'd to 1
	Float32				displayTime;		// 03C init'd to 2
};
static_assert(sizeof(BGSMessage) == 0x40);

// BGSRagdoll (148)
class BGSRagdoll : public TESForm
{
public:
	BGSRagdoll();
	~BGSRagdoll();

	TESModel	model;						// 018
	UInt32		unk030[(0x148-0x30) >> 2];	// 030
};
static_assert(sizeof(BGSRagdoll) == 0x148);

class BGSLightingTemplate : public TESForm
{
public:
	BGSLightingTemplate();
	~BGSLightingTemplate();

	UInt32	unk018[(0x44 - 0x18) >> 2];
};
static_assert(sizeof(BGSLightingTemplate) == 0x44);

// BGSMusicType (30)
class BGSMusicType : public TESForm
{
public:
	BGSMusicType();
	~BGSMusicType();

	TESSoundFile	soundFile;	// 018
	UInt32			unk024;
	UInt32			unk028;
	UInt32			unk02C;
};
static_assert(sizeof(BGSMusicType) == 0x30);

// BGSDefaultObjectManager, with help from "Luthien Anarion"
constexpr char kDefaultObjectNames[34][28] = {	// 0x0118C360 is an array of struct: { char * Name, UInt8 kFormType , UInt8 pad[3] }
      "Stimpack",
      "SuperStimpack",
      "RadX",
      "RadAway",
      "Morphine",
      "Perk Paralysis",
      "Player Faction",
      "Mysterious Stranger NPC",
      "Mysterious Stranger Faction",
      "Default Music",
      "Battle Music",
      "Death Music",
      "Success Music",
      "Level Up Music",
      "Player Voice (Male)",
      "Player Voice (Male Child)",
      "Player Voice (Female)",
      "Player Voice (Female Child)",
      "Eat Package Default Food",
      "Every Actor Ability",
      "Drug Wears Off Image Space",
      "Doctor's Bag",
      "Miss Fortune NPC",
      "Miss Fortune Faction",
      "Meltdown Explosion",
      "Unarmed Forward PA",
      "Unarmed Backward PA",
      "Unarmed Left PA",
      "Unarmed Right PA",
      "Unarmed Crouch PA",
      "Unarmed Counter PA",
      "Spotter Effect",
      "Item Detected Effect",
      "Cateye Mobile Effect (NYI)"
};

// BGSDefaultObjectManager (A0)
class BGSDefaultObjectManager : public TESForm
{
public:
	BGSDefaultObjectManager();
	~BGSDefaultObjectManager();

	static BGSDefaultObjectManager* GetSingleton();

	enum {
		kDefaultObject_Max = 34,
	};

	typedef TESForm *	FormArray[kDefaultObject_Max];

	struct FormStruct {
		TESForm*    Stimpak;
		TESForm*    SuperStimpak;
		TESForm*    RadX;
		TESForm*    RadAway;
		TESForm*    Morphine;
		TESForm*    PerkParalysis;
		TESForm*    PlayerFaction;
		TESForm*    MysteriousStranger;
		TESForm*    MysteriousStrangerFaction;
		TESForm*    DefaultMusic;
		TESForm*    BattleMusic;
		TESForm*    DefaultDeath;
		TESForm*    SuccessMusic;
		TESForm*    LevelUpMusic;
		TESForm*    PlayerVoiceMale;
		TESForm*    PlayerVoiceMaleChild;
		TESForm*    PlayerVoiceFemale;
		TESForm*    PlayerVoiceFemaleChild;
		TESForm*    EatPackageDefaultFood;
		TESForm*    EveryActorAbility;
		TESForm*    DrugWearOffImageSpace;
		// FNV
		TESForm*    DoctorsBag;
		TESForm*    MissFortuneNPC;
		TESForm*    MissFortuneFaction;
		TESForm*    MeltdownExplosion;
		TESForm*    UnarmedForwardPA;
		TESForm*    UnarmedBackwardPA;
		TESForm*    UnarmedLeftPA;
		TESForm*    UnarmedRightPA;
		TESForm*    UnarmedCrouchPA;
		TESForm*    UnarmedCounterPA;
		TESForm*    SpotterEffect;
		TESForm*    ItemDetectedEffect;
		TESForm*    CateyeMobileEffectNYI;
	};

	union DefaultObjects {
		FormStruct	asStruct;
		FormArray	asArray;
	};

	DefaultObjects	defaultObjects;	// 018
};

static_assert(sizeof(BGSDefaultObjectManager) == 0xA0);

enum EActionListForm
{
	eActionListForm_AddAt	= 00,
	eActionListForm_DelAt,
	eActionListForm_ChgAt,
	eActionListForm_GetAt,
	eActionListForm_Check,
	eActionListForm_Max,
};

enum EWhichListForm
{
	eWhichListForm_RaceHair					= 00,
	eWhichListForm_RaceEyes,
	eWhichListForm_RaceHeadPart,			// ? //
	eWhichListForm_BaseFaction,
	eWhichListForm_BaseRank,
	eWhichListForm_BasePackage,
	eWhichListForm_BaseSpellListSpell,
	eWhichListForm_BaseSpellListLevSpell,
	eWhichListForm_FactionRankName,
	eWhichListForm_FactionRankFemaleName,
	eWhichListForm_HeadParts,
	eWhichListForm_LevCreatureRef,
	eWhichListForm_LevCharacterRef,
	eWhichListForm_FormList,
	eWhichListForm_Max,
};

struct ConditionList : TList<Condition>
{
	bool Evaluate(TESObjectREFR* runOnRef, TESForm* arg2, bool* result, bool arg4);
};

// 20
struct FaceGenData
{
	UInt32		unk00;		// 00
	void*		unk04;		// 04
	UInt32		unk08;		// 08
	float**		values;		// 0C
	UInt32		useOffset;	// 10
	UInt32		maxOffset;	// 14
	UInt32		count;		// 18
	UInt32		size;		// 1C
};

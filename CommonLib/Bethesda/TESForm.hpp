#pragma once
#include "BaseFormComponent.hpp"
#include "BSString.hpp"
#include "BSSimpleList.hpp"
#include "NiTList.hpp"
#include "TESFile.hpp"
#include "NiTPointerMap.hpp"
#include "BSTCaseInsensitiveStringMap.hpp"
#include "NiTLargeArray.hpp"

#define IS_ID(form, type) (form->eTypeID == kType_##type)
#define NOT_ID(form, type) (form->eTypeID != kType_##type)

class TESLeveledList;
class TESFullName;
class Script;

// 0x18
class TESForm : public BaseFormComponent 
{
public:
	enum EnumType : UInt8 
	{
		kType_None = 0,					// 00
		kType_TES4,
		kType_Group,
		kType_GMST,
		kType_BGSTextureSet,
		kType_BGSMenuIcon,
		kType_TESGlobal,
		kType_TESClass,
		kType_TESFaction,				// 08
		kType_BGSHeadPart,
		kType_TESHair,
		kType_TESEyes,
		kType_TESRace,
		kType_TESSound,
		kType_BGSAcousticSpace,
		kType_TESSkill,
		kType_EffectSetting,			// 10
		kType_Script,
		kType_TESLandTexture,
		kType_EnchantmentItem,
		kType_SpellItem,
		kType_TESObjectACTI,
		kType_BGSTalkingActivator,
		kType_BGSTerminal,
		kType_TESObjectARMO,			// 18	inv object
		kType_TESObjectBOOK,			// 19	inv object
		kType_TESObjectCLOT,			// 1A	inv object
		kType_TESObjectCONT,
		kType_TESObjectDOOR,
		kType_IngredientItem,			// 1D	inv object
		kType_TESObjectLIGH,			// 1E	inv object
		kType_TESObjectMISC,			// 1F	inv object
		kType_TESObjectSTAT,			// 20
		kType_BGSStaticCollection,
		kType_BGSMovableStatic,			// 22
		kType_BGSPlaceableWater,
		kType_TESGrass,
		kType_TESObjectTREE,			// 24
		kType_TESFlora,
		kType_TESFurniture,
		kType_TESObjectWEAP,			// 28	inv object
		kType_TESAmmo,					// 29	inv object
		kType_TESNPC,					// 2A
		kType_TESCreature,				// 2B
		kType_TESLevCreature,			// 2C
		kType_TESLevCharacter,			// 2D
		kType_TESKey,					// 2E	inv object
		kType_AlchemyItem,				// 2F	inv object
		kType_BGSIdleMarker,			// 30
		kType_BGSNote,					// 31	inv object
		kType_BGSConstructibleObject,	// 32	inv object
		kType_BGSProjectile,
		kType_TESLevItem,				// 34	inv object
		kType_TESWeather,
		kType_TESClimate,
		kType_TESRegion,
		kType_NavMeshInfoMap,			// 38
		kType_TESObjectCELL,
		kType_TESObjectREFR,			// 3A
		kType_Character,				// 3B
		kType_Creature,					// 3C
		kType_MissileProjectile,		// 3D
		kType_GrenadeProjectile,		// 3E
		kType_BeamProjectile,			// 3F
		kType_FlameProjectile,			// 40
		kType_TESWorldSpace,
		kType_TESObjectLAND,
		kType_NavMesh,
		kType_TLOD,
		kType_TESTopic,
		kType_TESTopicInfo,
		kType_TESQuest,
		kType_TESIdleForm,				// 48
		kType_TESPackage,
		kType_TESCombatStyle,
		kType_TESLoadScreen,
		kType_TESLevSpell,
		kType_TESObjectANIO,
		kType_TESWaterForm,
		kType_TESEffectShader,
		kType_TOFT,						// 50	table of Offset (see OffsetData in Worldspace)
		kType_BGSExplosion,
		kType_BGSDebris,
		kType_TESImageSpace,
		kType_TESImageSpaceModifier,
		kType_BGSListForm,				// 55
		kType_BGSPerk,
		kType_BGSBodyPartData,
		kType_BGSAddonNode,				// 58
		kType_ActorValueInfo,
		kType_BGSRadiationStage,
		kType_BGSCameraShot,
		kType_BGSCameraPath,
		kType_BGSVoiceType,
		kType_BGSImpactData,
		kType_BGSImpactDataSet,
		kType_TESObjectARMA,			// 60
		kType_BGSEncounterZone,
		kType_BGSMessage,
		kType_BGSRagdoll,
		kType_DOBJ,
		kType_BGSLightingTemplate,
		kType_BGSMusicType,
		kType_TESObjectIMOD,			// 67	inv object
		kType_TESReputation,			// 68
		kType_ContinuousBeamProjectile,	// 69 Continuous Beam
		kType_TESRecipe,
		kType_TESRecipeCategory,
		kType_TESCasinoChips,			// 6C	inv object
		kType_TESCasino,
		kType_TESLoadScreenType,
		kType_MediaSet,
		kType_MediaLocationController,	// 70
		kType_TESChallenge,
		kType_TESAmmoEffect,
		kType_TESCaravanCard,			// 73	inv object
		kType_TESCaravanMoney,			// 74	inv object
		kType_TESCaravanDeck,
		kType_BGSDehydrationStage,
		kType_BGSHungerStage,
		kType_BGSSleepDeprevationStage,	// 78
		kType_Count
	};

	static const char* const TypeNames[kType_Count];

	enum EnumFlags : UInt32
	{
		IS_MASTER					= 1u << 0,
		IS_ALTERED					= 1u << 1,
		kModified_FactionFlags		= 1u << 2,
		INITIALIZED					= 1u << 3,
		UNK_4						= 1u << 4,
		DELETED						= 1u << 5,
		kFlags_Taken				= DELETED | IS_ALTERED,
		KNOWN						= 1u << 6,
		UNK_7						= 1u << 7,
		DROPPED						= 1u << 8,
		CASTS_SHADOWS				= 1u << 9,
		QUEST_ITEM					= 1u << 10,
		PERSISTENT					= 1u << 10,
		DISABLED					= 1u << 11,
		kFlags_IsPermanent			= 1u << 11,
		UNK_12						= 1u << 12,
		UNK_13						= 1u << 13,
		DONT_SAVE					= 1u << 14,
		TEMPORARY					= 1u << 14,
		VISIBLE_WHEN_DISTANT		= 1u << 15,
		HAVOK_DEATH					= 1u << 16,
		NEED_TO_CHANGE_PROCESS		= 1u << 17,
		COMPRESSED					= 1u << 18,
		UNK_19						= 1u << 19,
		CENTER_ON_CREATION			= 1u << 20,
		kFlags_IgnoreFriendlyHits	= 1u << 20,
		STILL_LOADING				= 1u << 21,
		BEING_DROPPED				= 1u << 22,
		UNK_23						= 1u << 23,
		kFlags_Destroyed			= 1u << 23,
		UNK_24						= 1u << 24,
		UNK_25						= 1u << 25,
		IS_VATS_TARGETTABLE			= 1u << 26,
		DISABLE_FADE				= 1u << 27,
		CHANGED_INVENTORY			= 1u << 27,
		UNK_28						= 1u << 28,
		UNK_29						= 1u << 29,
		TALKING_ACTIVATOR			= 1u << 30,
		CONTINUOUS_BROADCAST		= 1u << 30,
		UNK_31						= 1u << 31,
	};

	TESForm();
	virtual					~TESForm();
	virtual void			InitializeData();
	virtual void			ClearData();
	virtual bool			Unk_07();
	virtual bool			LoadForm(TESFile* apFile);			// func_010 in GECK
	virtual bool			LoadPartial(void* arg);				// points to LoadForm on TESForm
	virtual bool			AppendForm(TESFile* apFile);		// (ie SaveForm + append to apFile)
	virtual void			SaveForm();							// saves in same format as in .esp	//	func_013 in GECK
	// data buffer and buffer size stored in globals when done, doesn't return anything
	virtual bool			LoadForm2(TESFile* apFile);			// just calls LoadForm
	virtual void			WriteFormInfo(TESFile* apFile);		// does some saving stuff, then calls Fn0A
	virtual bool			Unk_0E(void* arg);					// prepares a GRUP formInfo
	virtual bool			Sort(TESForm* form);				// returns if the argument is "greater or equal" to this form
	virtual TESForm*		CreateForm(void* arg0, void* mapToAddTo);	// makes a new form,
	virtual void			Unk_11(void* arg);
	virtual void			AddChange(UInt32 changedFlags);
	virtual void			RemoveChange(UInt32 changedFlags);
	virtual UInt32			GetSaveSize(UInt32 changedFlags);	// bytes taken by the delta flags for this form, UNRELIABLE, not (always) overriden
	virtual void			AppendSaveLoadFormBuffer(void* arg);					// collect referenced forms?
	virtual void			SaveGame(UInt32 changedFlags);		// Used as part of CopyFromBase with LoadGame.
	virtual void			LoadGame(void* arg);				// load from BGSLoadFormBuffer arg
	virtual void			LoadGame2(UInt32 changedFlags);		// load from TESSaveLoadGame
	virtual void			InitLoadGame(void* arg);
	virtual void			Unk_1A(void* arg0, void* arg1);
	virtual void			Unk_1B(void* arg0, void* arg1);
	virtual void			Revert(UInt32 changedFlags);		// reset changes in form
	virtual void			Unk_1D(void* arg);
	virtual void			Unk_1E(void* arg);
	virtual bool			Unk_1F(void* arg);
	virtual void			Unk_20(void* arg);
	virtual void			FinishLoadGame(void* arg);
	virtual void			InitItem();
	virtual UInt32			GetSavedFormType();
	virtual void			GetFormDetailedString(BSStringT<char>* apDest);
	virtual bool			IsPermanentOrIsQuestItem();
	// Unk_26 though Unk_36 get or set flag bits
	virtual bool			HasTalkedToPC();					// 00000040
	virtual bool			GetHavokDeath();					// 00010000
	virtual bool			GetFlagBit16();						// 00010000
	virtual bool			IsNeedToChangeProcess();			// 00020000
	virtual bool			GetFlagBit17_2();					// 00020000
	virtual bool			GetFlag80000();						// 00080000
	virtual bool			GetFlag2000000();					// 02000000
	virtual bool			GetFlag40000000();					// 40000000
	virtual bool			IsShowInLocalMap();					// 00000200
	virtual void			SetCastsShadows(bool abShadowCaster);	// 00000200
	virtual bool			Unk_30();							// returns false
	virtual void			SetDelete(bool abDeleted);			// 00000020 then calls Fn12 MarkAsModified
	virtual void			SetAltered(bool abAltered);			// 00000002 with a lot of housekeeping
	virtual void			SetQuestItem(bool abQuestIem);
	virtual void			SetTalkedToPC(bool abTalkedTo);
	virtual void			SetHavokDeath(bool abDied);
	virtual void			SetNeedToChangeProcess(bool abChange);	// 00020000
	virtual void			Unk_37();		// write esp format
	virtual void			readOBNDSubRecord(TESFile* apFile);	// read esp format
	virtual bool			IsBoundObject();
	virtual bool			IsObject();
	virtual bool			IsMagicItem(); // EnchantmentItem, SpellItem, IngredientItem, AlchemyItem
	virtual bool			IsReference() const;
	virtual bool			IsArmorAddon();
	virtual bool			IsActorBase();
	virtual bool			IsMobileObject() const;
	virtual bool			IsActor();
	virtual UInt32			Unk_41();
	virtual void			Copy(const TESForm* apCopy);
	virtual bool			Compare(TESForm* apForm);
	virtual bool			BelongsInGroup(void* apGroupFORM);	// Checks the group is valid for the form
	virtual void			CreateGroupData(void* apOutGroupFORM, void* apParentGroup);	// Fills the groupInfo with info valid for the form
	virtual bool			Unk_46();
	virtual bool			Unk_47();
	virtual bool			Unk_48(UInt32 formType);	// returns if the same Type is passed in
	virtual bool			Activate(void* arg0, void* arg1, void* arg2, void* arg3, void* arg4);	// looks to be func33 in Oblivion
	virtual void			SetFormID(UInt32 aiID, bool abUpdateFile);
	virtual const char*		GetObjectTypeName();
	virtual const char*		GetEditorID() const;
	virtual bool			SetEditorID(const char* edid);
	// 4E

	enum
	{
		kModified_Flags = 0x00000001
		//	UInt32	flags;
	};

	EnumType		eTypeID;					// 004

	enum EnumJIPFormFlags : UInt8
	{
		kJIPFormFlag1_FastTravelInformed	= 1 << 0,
		kJIPFormFlag1_CellChangeInformed	= 1 << 1,
		kJIPFormFlag1_MenuInputInformed		= 1 << 2,
		kJIPFormFlag1_ScriptInformed		= kJIPFormFlag1_FastTravelInformed | kJIPFormFlag1_CellChangeInformed | kJIPFormFlag1_MenuInputInformed,
		kJIPFormFlag1_ScriptOnWait			= 1 << 3,

		kJIPFormFlag2_MessageDisabled		= 1 << 0,
		kJIPFormFlag2_SetStageHandlers		= kJIPFormFlag2_MessageDisabled,
		kJIPFormFlag2_ProjectileImpact		= kJIPFormFlag2_MessageDisabled,
		kJIPFormFlag2_OnEquipHandlers		= kJIPFormFlag2_MessageDisabled,
		kJIPFormFlag2_NoPCActivation		= 1 << 1,
		kJIPFormFlag2_EventDisabled			= 1 << 2,
		kJIPFormFlag2_ActivateDisabled		= 1 << 3,
		kJIPFormFlag2_UniqueItem			= 1 << 4,
		kJIPFormFlag2_IsAshPile				= 1 << 5,
		kJIPFormFlag2_InsertNode			= 1 << 6,
		kJIPFormFlag2_AttachModel			= 1 << 7,
		kJIPFormFlag2_InsertObject			= kJIPFormFlag2_InsertNode | kJIPFormFlag2_AttachModel,
	};

	EnumJIPFormFlags	eJIPFormFlags1;		// 005
	EnumJIPFormFlags	eJIPFormFlags2;		// 006
	EnumJIPFormFlags	eJIPFormFlags3;		// 007

	EnumFlags			eFlags;				// 008
	union
	{
		UInt32	uiFormID;					// 00C
		struct
		{
			UInt8 ucID[3];
			UInt8 ucModIndex;
		};
	};

	BSSimpleList<TESFile*> kMods;			// 010 ModReferenceList in Oblivion

	TESForm*					TryGetREFRParent() const;
	UInt8						GetModIndex() const;
	TESFullName*				GetFullName() const;
	const char*					GetTheName() const;
	std::string					GetStringRepresentation() const;
	bool						IsCloned() const { return ucModIndex == 0xFF; }
	Script*						GetScript();
	bool						IsItemPlayable();
	UInt32						GetItemValue();
	float						GetWeight();
	float						GetModifiedWeight();
	UInt8						GetOverridingModIdx();
	const char*					GetDescriptionText();
	const char*					RefToString();
	TESLeveledList*				GetLvlList();
	void						SetJIPFlag(UInt8 jipFlag, bool bSet);
	bool						IsQuestItem2() const	{ return eFlags & QUEST_ITEM; };
	bool						IsDisabled() const	{ return eFlags & kFlags_IsPermanent; };
	bool						IsTaken() const { return eFlags & kFlags_Taken; }
	bool						IsPersistent() const { return eFlags & PERSISTENT; }
	bool						IsTemporary() { return eFlags & TEMPORARY; }
	bool						IsDeleted() const { return eFlags & DELETED; }
	bool						IsDestroyed() { return eFlags & kFlags_Destroyed; }

	bool						IsInteractionDisabled() const { return eJIPFormFlags2 & kJIPFormFlag2_NoPCActivation; };
	static void					DoAddForm(TESForm* newForm, bool bPersist = true, bool record = true);
	TESForm*					CloneForm(bool bPersist = true) const;
	bool						IsInventoryObject() const { return CdeclCall<char>(0x00481F30, eTypeID); }
	bool						IsInventoryObjectAlt(); 
	bool						IsCreated() const { return ucModIndex == 0xFF; }
	__forceinline bool			CanContainItems() const { return eTypeID == kType_TESObjectCONT || eTypeID == kType_TESNPC || eTypeID == kType_TESCreature; }
	__forceinline bool			IsActivator() const { return eTypeID == kType_TESObjectACTI; }
	bool						IsWeapon() { return eTypeID == kType_TESObjectWEAP; }
	bool						IsArmor() { return eTypeID == kType_TESObjectARMO; }

	__forceinline static NiTPointerMap<UInt32, TESForm*>* GetAll() { return *reinterpret_cast<NiTPointerMap<UInt32, TESForm*>**>(0x11C54C0); }
	__forceinline static TESForm* GetByID(const char* editorID) { return CdeclCall<TESForm*>(0x483A00, editorID); };
	__forceinline static TESForm* GetByID(UInt32 refID) { return *GetAll()->Lookup(refID); };
	static TESForm*				GetByID(const char* mod, UInt32 refID);
	static TESForm*				GetByID(const char* mod, const char* refID);


	inline UInt32				GetFormID() const { return uiFormID; }

	static NiTPointerMap<UInt32, TESForm*>** const pAllForms;
	static BSTCaseInsensitiveStringMap<TESForm*>** const pAllFormsByEditorID;
	static NiTLargePrimitiveArray<TESForm*>* const pAlteredForms;

	bool IsWeapon() const { return eTypeID == kType_TESObjectWEAP; }
	bool IsArmor() const { return eTypeID == kType_TESObjectARMO; }

	TESForm*	CloneForm(bool bPersist = true);
	bool		FormMatches(TESForm* toMatch) const;
	void		SetTemporary();
	TESFile*	GetFile(UInt32 idx) const;
	UInt32		GetRelativeFormID() const;
	UInt32		GetLoadFormID() const;
	static void	__fastcall SetFormIDEx(TESForm* apThis, void*, UInt32 aiID, bool abUpdateFile);
};
static_assert(sizeof(TESForm) == 0x18);
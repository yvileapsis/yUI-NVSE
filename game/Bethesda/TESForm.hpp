#pragma once

#include "BaseFormComponent.hpp"
#include "TESFullName.hpp"
#include "BSString.hpp"
#include "BSSimpleList.hpp"
#include "TESFile.hpp"
#include "BSTCaseInsensitiveStringMap.hpp"
#include "NiTLargeArray.hpp"

#define IS_ID(form, type) (form->ucTypeID == kFormType_##type)
#define NOT_ID(form, type) (form->ucTypeID != kFormType_##type)

enum FormType {
	kFormType_None = 0,					// 00
	kFormType_TES4,
	kFormType_Group,
	kFormType_GMST,
	kFormType_BGSTextureSet,
	kFormType_BGSMenuIcon,
	kFormType_TESGlobal,
	kFormType_TESClass,
	kFormType_TESFaction,					// 08
	kFormType_BGSHeadPart,
	kFormType_TESHair,
	kFormType_TESEyes,
	kFormType_TESRace,
	kFormType_TESSound,
	kFormType_BGSAcousticSpace,
	kFormType_TESSkill,
	kFormType_EffectSetting,					// 10
	kFormType_Script,
	kFormType_TESLandTexture,
	kFormType_EnchantmentItem,
	kFormType_SpellItem,
	kFormType_TESObjectACTI,
	kFormType_BGSTalkingActivator,
	kFormType_BGSTerminal,
	kFormType_TESObjectARMO,					// 18	inv object
	kFormType_TESObjectBOOK,						// 19	inv object
	kFormType_TESObjectCLOT,					// 1A	inv object
	kFormType_TESObjectCONT,
	kFormType_TESObjectDOOR,
	kFormType_IngredientItem,				// 1D	inv object
	kFormType_TESObjectLIGH,					// 1E	inv object
	kFormType_TESObjectMISC,						// 1F	inv object
	kFormType_TESObjectSTAT,					// 20
	kFormType_BGSStaticCollection,
	kFormType_BGSMovableStatic,
	kFormType_BGSPlaceableWater,
	kFormType_TESGrass,
	kFormType_TESObjectTREE,
	kFormType_TESFlora,
	kFormType_TESFurniture,
	kFormType_TESObjectWEAP,					// 28	inv object
	kFormType_TESAmmo,						// 29	inv object
	kFormType_TESNPC,						// 2A
	kFormType_TESCreature,					// 2B
	kFormType_TESLevCreature,			// 2C
	kFormType_TESLevCharacter,			// 2D
	kFormType_TESKey,						// 2E	inv object
	kFormType_AlchemyItem,				// 2F	inv object
	kFormType_BGSIdleMarker,				// 30
	kFormType_BGSNote,						// 31	inv object
	kFormType_BGSConstructibleObject,		// 32	inv object
	kFormType_BGSProjectile,
	kFormType_TESLevItem,				// 34	inv object
	kFormType_TESWeather,
	kFormType_TESClimate,
	kFormType_TESRegion,
	kFormType_NavMeshInfoMap,						// 38
	kFormType_TESObjectCELL,
	kFormType_TESObjectREFR,				// 3A
	kFormType_Character,						// 3B
	kFormType_Creature,						// 3C
	kFormType_MissileProjectile,						// 3D
	kFormType_GrenadeProjectile,						// 3E
	kFormType_BeamProjectile,						// 3F
	kFormType_FlameProjectile,						// 40
	kFormType_TESWorldSpace,
	kFormType_TESObjectLAND,
	kFormType_NavMesh,
	kFormType_TLOD,
	kFormType_TESTopic,
	kFormType_TESTopicInfo,
	kFormType_TESQuest,
	kFormType_TESIdleForm,						// 48
	kFormType_TESPackage,
	kFormType_TESCombatStyle,
	kFormType_TESLoadScreen,
	kFormType_TESLevSpell,
	kFormType_TESObjectANIO,
	kFormType_TESWaterForm,
	kFormType_TESEffectShader,
	kFormType_TOFT,						// 50	table of Offset (see OffsetData in Worldspace)
	kFormType_BGSExplosion,
	kFormType_BGSDebris,
	kFormType_TESImageSpace,
	kFormType_TESImageSpaceModifier,
	kFormType_BGSListForm,					// 55
	kFormType_BGSPerk,
	kFormType_BGSBodyPartData,
	kFormType_BGSAddonNode,				// 58
	kFormType_ActorValueInfo,
	kFormType_BGSRadiationStage,
	kFormType_BGSCameraShot,
	kFormType_BGSCameraPath,
	kFormType_BGSVoiceType,
	kFormType_BGSImpactData,
	kFormType_BGSImpactDataSet,
	kFormType_TESObjectARMA,						// 60
	kFormType_BGSEncounterZone,
	kFormType_BGSMessage,
	kFormType_BGSRagdoll,
	kFormType_DOBJ,
	kFormType_BGSLightingTemplate,
	kFormType_BGSMusicType,
	kFormType_TESObjectIMOD,					// 67	inv object
	kFormType_TESReputation,				// 68
	kFormType_ContinuousBeamProjectile,						// 69 Continuous Beam
	kFormType_TESRecipe,
	kFormType_TESRecipeCategory,
	kFormType_TESCasinoChips,				// 6C	inv object
	kFormType_TESCasino,
	kFormType_TESLoadScreenType,
	kFormType_MediaSet,
	kFormType_MediaLocationController,	// 70
	kFormType_TESChallenge,
	kFormType_TESAmmoEffect,
	kFormType_TESCaravanCard,				// 73	inv object
	kFormType_TESCaravanMoney,				// 74	inv object
	kFormType_TESCaravanDeck,
	kFormType_BGSDehydrationStage,
	kFormType_BGSHungerStage,
	kFormType_BGSSleepDeprevationStage,	// 78
};

class TESForm : public BaseFormComponent {
public:
	TESForm();
	
	virtual					~TESForm();
	virtual void			InitializeData();
	virtual void			ClearData();
	virtual bool			Unk_07();
	virtual bool			Load(TESFile* apFile);		// func_010 in GECK
	virtual bool			LoadPartial(void* arg);					// points to LoadForm on TESForm
	virtual bool			AppendForm(TESFile* apFile);		// (ie SaveForm + append to apFile)
	virtual void			SaveForm();						// saves in same format as in .esp	//	func_013 in GECK
	// data buffer and buffer size stored in globals when done, doesn't return anything
	virtual bool			LoadForm2(TESFile* apFile);		// just calls LoadForm
	virtual void			WriteFormInfo(TESFile* apFile);	// does some saving stuff, then calls Fn0A
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
	virtual void			GetFormDetailedString(BSStringT* apDest);
	virtual bool			IsPermanentOrIsQuestItem();
	virtual bool			HasTalkedToPC();		// 00000040
	virtual bool			GetHavokDeath();		// 00010000
	virtual bool			Unk_28();		// 00010000
	virtual bool			IsNeedToChangeProcess();		// 00020000
	virtual bool			Unk_2A();		// 00020000
	virtual bool			GetFlag80000();		// 00080000
	virtual bool			GetFlag2000000();		// 02000000
	virtual bool			GetFlag40000000();		// 40000000
	virtual bool			IsShowInLocalMap();		// 00000200
	virtual void			SetCastsShadows(bool abShadowCaster);	// 00000200
	virtual bool			Unk_30();		// returns false | TESOBjectLIGH returns some pos
	virtual void			SetDelete(bool abDeleted);	// 00000020 then calls Fn12 MarkAsModified
	virtual void			SetAltered(bool abAltered);	// 00000002 with a lot of housekeeping
#if RUNTIME
	virtual void			SetQuestItem(bool abQuestIem);
#else
	virtual bool			Unk_33();
#endif
	virtual void			SetTalkedToPC(bool abTalkedTo);
	virtual void			SetHavokDeath(bool abDied);
	virtual void			SetNeedToChangeProcess(bool abChange);	// 00020000
	virtual void			Unk_37();		// write esp format
	virtual void			readOBNDSubRecord(TESFile* apFile);	// read esp format
	virtual bool			IsBoundObject();
	virtual bool			IsObject();
	virtual bool			IsMagicItem();
#if RUNTIME
	virtual bool			IsReference() const;
#else
	virtual bool		Unk_3C();
	bool GetIsReference() const { return typeID == kFormType_TESObjectREFR; }
#endif
	virtual bool			IsArmorAddon();
	virtual bool			IsActorBase();
	virtual bool			IsMobileObject() const;
	virtual bool			IsActor();
	virtual UInt32			Unk_41();
	virtual void			Copy(TESForm* apCopy);
	virtual bool			Compare(TESForm* apForm);
	virtual bool			BelongsInGroup(void* apGroupFORM);	// Checks the group is valid for the form
	virtual void			CreateGroupData(void* apOutGroupFORM, void* apParentGroup);	// Fills the groupInfo with info valid for the form
	virtual bool			Unk_46();
	virtual bool			Unk_47();
	virtual bool			Unk_48(UInt32 formType);	// returns if the same FormType is passed in
	virtual bool			Activate(void* arg0, void* arg1, void* arg2, void* arg3, void* arg4);
	virtual void			SetFormID(UInt32 aiID, bool abUpdateFile);
	virtual const char*		GetObjectTypeName();
	virtual const char*		GetName() const;
	virtual bool			SetEditorID(const char* edid);
	// 4E

	// Requires JohnnyGuitar NVSE for this to work at runtime.
	const char* GetEditorID() const;

	struct EditorData {
		BSStringT	strEditorID;
		UInt32		vcMasterFormID;
		UInt32		vcRevision;
	};
	// 10

	enum Flags : UInt32 {
		IS_MASTER				= 1u << 0,
		IS_ALTERED				= 1u << 1,
		INITIALIZED				= 1u << 3,
		UNK_4					= 1u << 4,
		DELETED					= 1u << 5,
		KNOWN					= 1u << 6,
		UNK_7					= 1u << 7,
		DROPPED					= 1u << 8,
		CASTS_SHADOWS			= 1u << 9,
		QUEST_ITEM				= 1u << 10,
		PERSISTENT				= 1u << 10,
		DISABLED				= 1u << 11,
		UNK_12					= 1u << 12,
		UNK_13					= 1u << 13,
		DONT_SAVE				= 1u << 14,
		TEMPORARY				= 1u << 14,
		VISIBLE_WHEN_DISTANT	= 1u << 15,
		HAVOK_DEATH				= 1u << 16,
		NEED_TO_CHANGE_PROCESS	= 1u << 17,
		COMPRESSED				= 1u << 18,
		UNK_19					= 1u << 19,
		CENTER_ON_CREATION		= 1u << 20,
		STILL_LOADING			= 1u << 21,
		BEING_DROPPED			= 1u << 22,
		UNK_23					= 1u << 23,
		UNK_24					= 1u << 24,
		UNK_25					= 1u << 25,
		IS_VATS_TARGETTABLE		= 1u << 26,
		DISABLE_FADE			= 1u << 27,
		CHANGED_INVENTORY		= 1u << 27,
		UNK_28					= 1u << 28,
		UNK_29					= 1u << 29,
		TALKING_ACTIVATOR		= 1u << 30,
		CONTINUOUS_BROADCAST	= 1u << 30,
		UNK_31					= 1u << 31,

		TAKEN = DELETED | IS_ALTERED,
	};

	enum {
		kModified_Flags = 0x00000001
		//	UInt32	flags;
	};

	UInt8					ucTypeID;
	Bitfield32				uiFormFlags;
	union {
		UInt32					uiFormID;
		struct
		{
			UInt8	id[3];
			UInt8	modIndex;
		};
	};
#ifdef EDITOR
	EditorData	editorData;			// +10
#endif
	BSSimpleList<TESFile*>	kFiles;

	inline UInt32	GetFormID() const { return uiFormID; }
	TESForm*		TryGetREFRParent();
	UInt8			GetModIndex() const;
	TESFullName*	GetFullName() const;
	const char*		GetTheName() const;
	std::string		GetStringRepresentation() const;

	static NiTPointerMap<UInt32, TESForm*>**		const pAllForms;
	static BSTCaseInsensitiveStringMap<TESForm*>**	const pAllFormsByEditorID;
	static NiTLargePrimitiveArray<TESForm*>*		const pAlteredForms;


	bool IsWeapon() const { return ucTypeID == kFormType_TESObjectWEAP; }
	bool IsArmor() const { return ucTypeID == kFormType_TESObjectARMO; }

	bool GetDeleted() const { return uiFormFlags.IsSet(DELETED); }
	void SetDeleted(bool abSet) { uiFormFlags.SetBit(DELETED, abSet); }

	bool GetTaken() const { return uiFormFlags.IsSet(TAKEN); }
	void SetTaken(bool abSet) { uiFormFlags.SetBit(TAKEN, abSet); }

	bool GetPersistent() const { return uiFormFlags.IsSet(PERSISTENT); }
	void SetPersistent(bool abSet) { uiFormFlags.SetBit(PERSISTENT, abSet); }

	bool GetDisabled() const { return uiFormFlags.IsSet(DISABLED); }
	void SetDisabled(bool abSet) { uiFormFlags.SetBit(DISABLED, abSet); }

	bool GetDontSave() const { return uiFormFlags.IsSet(DONT_SAVE); }
	void SetDontSave(bool abSet) { uiFormFlags.SetBit(DONT_SAVE, abSet); }

	bool GetTemporary() const { return uiFormFlags.IsSet(TEMPORARY); }
	void SetTemporary(bool abSet) { uiFormFlags.SetBit(TEMPORARY, abSet); }

	void SetBeingDropped(bool abSet) { uiFormFlags.SetBit(BEING_DROPPED, abSet); }

	// adds a new form to the game (from CloneForm or LoadForm)
	void DoAddForm(TESForm* newForm, bool bPersist = true, bool record = true) const;
	// return a new base form which is the clone of this form
	TESForm* CloneForm(bool bPersist = true);
	bool     IsInventoryObject() const;

	bool FormMatches(TESForm* toMatch) const;

	MEMBER_FN_PREFIX(TESForm);
#if RUNTIME
	void SetTemporary();
#endif
	TESFile* GetFile(UInt32 idx) const;
	UInt32 GetRelativeFormID() const;
	UInt32 GetLoadFormID() const;

	static void	__fastcall SetFormIDEx(TESForm* apThis, void*, UInt32 aiID, bool abUpdateFile);
};

ASSERT_SIZE(TESForm, 0x18);

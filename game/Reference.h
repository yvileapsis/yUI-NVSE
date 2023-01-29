#pragma once
#include <Object.h>
#include <GameProcess.h>
#include <Sound.h>

typedef std::unordered_map<TESForm*, InventoryItemData> InventoryItemsMap;

class TESObjectREFR : public TESForm
{
public:

	TESObjectREFR();
	~TESObjectREFR();

	virtual void				Unk_4E(void);	// GetStartingPosition(Position, Rotation, WorldOrCell)
	virtual void				Unk_4F(void);
	virtual void				Unk_50(void);
	virtual void				Unk_51(void);
	virtual bool				CastShadows(void);
	virtual void				Unk_53(void);
	virtual void				Unk_54(void);
	virtual void				Unk_55(void);
	virtual void				Unk_56(void);
	virtual bool				IsObstacle();
	virtual void				Unk_58(void);
	virtual void				Unk_59(void);
	virtual void				Unk_5A(void);
	virtual void				GetStartingAngle(void);
	virtual void				GetStartingPos(void);
	virtual void				Unk_5D(void);
	virtual void				Unk_5E(void);
	virtual TESObjectREFR*		RemoveItem(TESForm* toRemove, BaseExtraList* extraList, UInt32 count, UInt32 keepOwner, UInt32 drop, TESObjectREFR* destRef, 
											UInt32 unk6, UInt32 unk7, UInt32 unk8, UInt8 unk9);	// 40 unk2 quantity? Returns the reference assigned to the removed item.
	virtual void				GetBodyPartData(void);
	virtual bool				EquipObject(TESForm* item, UInt32 count, ExtraDataList* xData, bool lockEquip);
	virtual void				Unk_62(void);	// Linked to Unequip (and or equip maybe)
	virtual void				Unk_63(void);
	virtual void				AddItem(TESForm* item, ExtraDataList* xDataList, UInt32 Quantity);	// Needs confirmation
	virtual void				Unk_65(void);
	virtual void				GetMagicCaster(void);
	virtual void				GetMagicTarget(void);							// Actor: GetMagicEffectList
	virtual bool				GetIsChildSize(bool checkHeight);		// 068 Actor: GetIsChildSize
	virtual UInt32				GetActorUnk0148(void);					// result can be interchanged with baseForm, so TESForm* ?
	virtual void				SetActorUnk0148(UInt32 arg0);
	virtual void				GetBSFaceGenNiNodeBiped(void);
	virtual void				GetBSFaceGenNiNodeSkinned(void);				// REFR: GetBSFaceGenNiNodeSkinned
	virtual void				Unk_6D(void);				// REFR: calls 006C
	virtual void				Unk_6E(void);				// MobileActor: calls 006D then NiNode::Func0040
	virtual void				Unk_6F(void);
	virtual bool				Unload3D();
	virtual void				AnimateNiNode(void);					// same in FOSE ! identical to Func0052 in OBSE which says (inits animation-related data, and more)
	virtual void				GenerateNiNode(bool arg0);				// same in FOSE !
	virtual void				Set3D(NiNode* niNode, bool unloadArt);	// same in FOSE !
	virtual NiNode *			GetNiNode_v(void);						// same in FOSE !
	virtual void				Unk_75(void);
	virtual void				Unk_76(void);
	virtual void				Unk_77(void);
	virtual void				Unk_78(void);
	virtual AnimData*			GetAnimData(void);			// 0079
	virtual ValidBip01Names*	GetValidBip01Names(void);	// 007A	Character only
	virtual ValidBip01Names*	CallGetValidBip01Names(void);
	virtual void				SetValidBip01Names(ValidBip01Names validBip01Names);
	virtual NiPoint3*			GetPos(void);				// GetPos or GetDistance
	virtual void				Unk_7E(UInt32 arg0);
	virtual void				Unk_7F(void);
	virtual void				Unk_80(UInt32 arg0);
	virtual void				Unk_81(UInt32 arg0);
	virtual void				Unk_82(void);
	virtual NiNode*				GetProjectileNode(void);
	virtual void				Unk_84(UInt32 arg0);
	virtual UInt32				GetSitSleepState(void);				// 0 or GetActor::Unk01AC
	virtual bool				IsCharacter();				// return false for Actor and Creature, true for character and PlayerCharacter
	virtual bool				IsCreature();
	virtual bool				IsExplosion();
	virtual bool				IsProjectile();
	virtual void				SetParentCell(void);				// SetParentCell (Interior only ?)
	virtual bool				IsDying(bool essentialUnconcious = false);			// IsDead = HasNoHealth (baseForm health <= 0 or Flags bit23 set)
	virtual bool				GetHasKnockedState(void);
	virtual bool				Unk_8D(void);
	virtual void				Unk_8E(void);
	virtual void				Unk_8F(void);
	virtual void				StartHighProcess(void);

	struct RenderState
	{
		TESObjectREFR*	waterRef;		// 00
		UInt32			unk04;			// 04	0-0x13 when fully-underwater; exterior only
		float			waterLevel;		// 08
		float			unk0C;			// 0C
		UInt32			unk10;			// 10
		NiNode*			niNode14;		// 14
		NiNode*			niNode18;		// 18
	};

	struct EditorData {
		UInt32			unk00;	// 00
	};
	// 0C

	TESChildCell		childCell;				// 018

	TESSound*			loopSound;				// 01C

	TESForm*			baseForm;				// 020
	
	NiPoint3			rot;		// 024 - either public or accessed via simple inline accessor common to all child classes
	NiPoint3			pos;		// 030 - seems to be private
	Float32				scale;					// 03C 

	TESObjectCELL*		parentCell;				// 040
	ExtraDataList		extraDataList;			// 044
	RenderState*		renderState;			// 064	- (05C in FOSE)

	ScriptEventList*		GetEventList() const;

	bool					IsTaken() const { return flags & kFormFlags_Taken; }
	bool					IsPersistent() const { return flags & kFormFlags_Persistent; }
	bool					IsTemporary() { return flags & kFormFlags_Temporary; }
	bool					IsDeleted() const { return flags & kFormFlags_Deleted; }
	bool					IsDestroyed() { return flags & kFormFlags_Destroyed; }

	NiPoint3*				PosVector() { return (NiPoint3*)&pos; }
	CoordXY*				PosXY() { return (CoordXY*)&pos; }
	
	bool					Update3D();
	bool					Update3D_v1c();	// Less worse version as used by some modders
	TESContainer*			GetContainer();
	bool					IsMapMarker();
	bool					GetInventoryItems(InventoryItemsMap &invItems);

	static TESObjectREFR*	Create(bool bTemp = false);

	bool					GetDisabled();
	ExtraContainerChanges*	GetOrCreateContainerChanges();
	InventoryChangesList*	GetContainerChangesList();
	SInt32					GetItemCount(TESForm* form) { return ThisCall<SInt32>(0x575610, this, form); };
	void					AddItemAlt(TESForm* item, UInt32 count, float condition, bool doEquip);
	bool					GetInventoryItems(UInt8 typeID);
	TESObjectCELL*			GetParentCell() const { return parentCell; };
	TESWorldSpace*			GetParentWorld() { return ThisCall<TESWorldSpace*>(0x575D70, this); };
	bool __fastcall			GetInSameCellOrWorld(TESObjectREFR* target) const;
	Float32 __vectorcall	GetDistance(TESObjectREFR* target);
	Float32 __vectorcall	GetDistance2D(TESObjectREFR* target);
	void					SetPos(NiPoint3& posVector);
	void					SetAngle(NiPoint3& rotVector);
	Float32					GetHeadingAngle(TESObjectREFR* target);
	bool					MoveToCell(TESForm* worldOrCell, NiPoint3& posVector);
	void					DeleteReference();
	TESObjectREFR*			GetMerchantContainer();
	float					GetWaterImmersionPerc();
	bool					IsMobile();
	bool					IsGrabbable();
	bool					ValidForHooks();
	NiAVObject*				GetNiBlock(const char* blockName);
	NiNode* __fastcall		GetNode(const char* nodeName);
	hkpRigidBody*			GetRigidBody(const char* nodeName);
	bool					RunScriptSource(const char* sourceStr);
	ExtraLockData*			GetLockData() { return ThisCall<ExtraLockData*>(0x569160, this); }
	void					SetScale(float scale);
	bool					IsOwnedByActor(Actor* actor, bool includeFactionOwnership) { return ThisCall<bool>(0x5785E0, this, actor, includeFactionOwnership); };
	TESObjectREFR*			ResolveOwnership() { return ThisCall<TESObjectREFR*>(0x567790, this); };
	bool					IsInInterior() { return ThisCall<bool>(0x575D10, this); };
	NiNode*					GetNiNode();
	NiAVObject*				GetNifBlock(UInt32 pcNode, const char* blockName);
	bool					IsCrimeOrEnemy();
	NiPoint3				GetCenter() const;
	NiPoint3				GetDimensions() const;
	__forceinline Float32	GetScale() { return ThisCall<float>(0x567400, this); }
	TESObjectREFR*			ResolveAshpile();
	bool					IsLocked();

	__forceinline bool		Activate(Actor* activator, UInt32 unk1, UInt32 unk2, UInt32 unk3) { return ThisCall<bool>(0x573170, this, activator, unk1, unk2, unk3); }

	Float64					GetInventoryWeight();

	std::vector<InventoryChanges*> GetAllItems(UInt32 checkLinkedRec = 0);

	void					OpenCloseContainer(bool open = false, bool sounds = true);
};
static_assert(sizeof(TESObjectREFR) == 0x68);

TESForm* GetPermanentBaseForm(TESObjectREFR* thisObj);	// For LevelledForm, find real baseForm, not temporary one.

class MobileObject : public TESObjectREFR
{
public:
	MobileObject();
	~MobileObject();

	virtual void		Unk_91(void);
	virtual void		Unk_92(void);
	virtual void		Unk_93(void);
	virtual void		Move(void);
	virtual void		Jump(void);
	virtual void		nullsub_96(void);
	virtual void		ProcessAI(void);
	virtual void		Unk_98(void);
	virtual void		Unk_99(void);
	virtual void		Unk_9A(void);
	virtual void		Unk_9B(void);
	virtual void		Unk_9C(void);
	virtual void		Unk_9D(void);
	virtual void		Unk_9E(void);
	virtual void		Unk_9F(void);
	virtual void		StartConversation(void);	// StartConversation(targetActor, subjectLocationData, targetLocationData, headTrack, allowMovement, arg, topicID, arg, arg
	virtual void		DoSpeechLoadLipFiles(void);
	virtual void		Unk_A2(void);
	virtual void		Unk_A3(void);
	virtual void		Unk_A4(void);
	virtual void		Unk_A5(void);
	virtual void		Unk_A6(void);
	virtual void		Unk_A7(void);
	virtual void		Unk_A8(void);
	virtual void		Unk_A9(void);
	virtual void		SetPos(void);
	virtual void		Unk_AB(void);
	virtual void		HandleRunDetection(void);
	virtual void		Unk_AD(void);
	virtual void		Unk_AE(void);
	virtual void		AdjustRot(void);
	virtual void		Unk_B0(void);
	virtual void		SetRotation(void);
	virtual void		Unk_B2(void);
	virtual void		Unk_B3(void);
	virtual void		GetSpeechExpression(void);
	virtual void		SetExpression(void);
	virtual void		GetSpeechVolume(void);
	virtual void		SetEmotionValue(void);
	virtual void		Unk_B8(void);
	virtual void		Unk_B9(void);
	virtual void		IsLifeStateDying(void);
	virtual void		Unk_BB(void);
	virtual void		Unk_BC(void);
	virtual void		Unk_BD(void);
	virtual void		Update(void);
	virtual void		Unk_BF(void);
	virtual void		Unk_C0(void);
	
	BaseProcess*	baseProcess;						// 068
	UInt32			unk06C;								// 06C - loaded	set to the talking actor ref when initialising ExtraTalkingActor
	Actor*			conversationActor;					// 070 - loaded
	UInt32			nextHeadTrackUpdateTimer;			// 074 - loaded
	UInt32			voiceTimer;							// 078 - loaded
	UInt8			unk07C;								// 07C - loaded
	UInt8			unk07D;								// 07D - loaded
	UInt8			unk07E;								// 07E - loaded
	UInt8			unk07F;								// 07F - loaded
	UInt8			unk080;								// 080 - loaded
	UInt8			isTalkingThroughActivator;			// 081 - loaded
	UInt8			unk082;								// 082 - cleared when starting combat on player
	UInt8			unk083;								// 083 - loaded
	UInt8			unk084;								// 084 - loaded
	UInt8			unk085;								// 085 - loaded
	UInt8			extraInfoGeneralTopicDataFlags;		// 086 - loaded
	UInt8			unk087;								// 087	Init'd to the inverse of NoLowLevelProcessing
};
static_assert(sizeof(MobileObject) == 0x088);

class MagicCaster
{
public:
	MagicCaster();
	~MagicCaster();

	enum State
	{
		kCastState_Inactive = 0,
		kCastState_Aim = 1,
		kCastState_Cast = 2,
		kCastState_ReleaseCast = 3,
		kCastState_FindTargets = 4,
		kCastState_SpellDisabled = 5,
		kCastState_AlreadyCasting = 6,
		kCastState_CannotCast = 7
	};

	virtual void			AddAbility(SpellItem* splItem, bool arg2);															// 000
	virtual void			AddAddiction(SpellItem* splItem, bool arg2);														// 004
	virtual void			AddEffect(SpellItem* splItem, bool arg2);															// 008
	virtual void			CastSpell(MagicItem* spell, bool arg2, MagicTarget* target, float arg4, bool arg5);/*00C*/
	virtual void			AddDisease(SpellItem* splItem, MagicTarget* target, bool arg3);/*010*/
	virtual void			AddFormEffects(MagicItem* magItem, MagicItemForm* itemForm, bool arg3);/*014*/
	virtual MagicTarget*	PickMagicTarget();/*018*/
	virtual void			Unk_07();/*01C*/
	virtual void			Unk_08();/*020*/
	virtual void			NullSub(UInt32 arg1, UInt32 arg2);/*024*/
	virtual bool			Unk_0A(UInt32 arg1, float* arg2, UInt32* arg3, UInt32 arg4);/*028*/
	virtual Actor*			GetActor();/*02C*/
	virtual NiNode*			GetMagicNode();/*030*/
	virtual MagicItem*		GetMagicItem();/*034*/
	virtual bool			ApplyEffectToTarget(ActiveEffect* activeEffect);/*038*/
	virtual float			Unk_0F(UInt8 arg1, float arg2);/*03C*/
	virtual void			SetMagicItem(MagicItem* spell);/*040*/
	virtual MagicTarget*	GetMagicTarget();/*044*/
	virtual void			SetMagicTarget(MagicTarget* magicTarget);/*048*/
	virtual ActiveEffect*	CreateActiveEffect(MagicItem* magicItem, EffectItem* effItem, TESForm* itemForm);				// 04C

	void*	unk004;	// 004
	State	state;	// 008
};
static_assert(sizeof(MagicCaster) == 0xC);

typedef TList<ActiveEffect> EffectNode;

class MagicTarget
{
public:
	MagicTarget();
	~MagicTarget();

	virtual void			Destructor(void);
	virtual TESObjectREFR*	GetParent(void);
	virtual EffectNode*		GetEffectList(void);

	UInt8					byt004;		// 004 
	UInt8					byt005;		// 005 
	UInt8					byt006;		// 006
	UInt8					byt007;		// 007
	TList<void*>			lst008;		// 008

	void					RemoveEffect(EffectItem *effItem);
	void					StopEffect(void* arg0, bool arg1) { ThisStdCall(0x8248E0, this, arg0, arg1); }
};
static_assert(sizeof(MagicTarget) == 0x10);

class hkaRaycastInterface
{
public:
	hkaRaycastInterface();
	~hkaRaycastInterface();
	virtual hkaRaycastInterface*	Destroy(bool doFree);
	virtual void					Unk_01(void* arg0);
	virtual void					Unk_02();

	// Assumed to be 0x010 bytes due to context where the vftable is used
	UInt32							unk000[(0x010 - 0x004) >> 2];	// 0000
};
static_assert(sizeof(hkaRaycastInterface) == 0x010);

class bhkRagdollController : public hkaRaycastInterface
{
public:
	bhkRagdollController();
	~bhkRagdollController();

	UInt32	unk000[(0x021C - 0x010) >> 2];	// 0010
	UInt8	fill021C[3];					// 021C
	bool	bool021F;						// 021F	when null assume FIK status is false
	bool	fikStatus;						// 0220
	UInt8	fill0221[3];					// 0221
};
static_assert(sizeof(bhkRagdollController) == 0x0224);

class ActorMover	// I need to call Func008
{
public:
	virtual void		Unk_00(void);
	virtual void		Unk_01(void);
	virtual void		Unk_02(void);
	virtual void		Unk_03(void);
	virtual void		Unk_04(void);
	virtual void		Unk_05(void);
	virtual void		Unk_06(void);
	virtual void		Unk_07(void);
	virtual UInt32		GetMovementFlags(void);	// for PlayerMover, it is GetMovementFlags
		// bit 11 = swimming 
		// bit 9 = sneaking
		// bit 8 = run
		// bit 7 = walk
		// bit 0 = keep moving (Q)
};


enum ActorValueCode
{
	kAVCode_None = -1,
	kAVCode_Aggression = 0x0,
	kAVCode_Confidence = 0x1,
	kAVCode_Energy = 0x2,
	kAVCode_Responsibility = 0x3,
	kAVCode_Mood = 0x4,

	kAVCode_Strength = 0x5,
	kAVCode_Perception = 0x6,
	kAVCode_Endurance = 0x7,
	kAVCode_Charisma = 0x8,
	kAVCode_Intelligence = 0x9,
	kAVCode_Agility = 0xA,
	kAVCode_Luck = 0xB,
	kAVCode_SpecialStart = kAVCode_Strength,
	kAVCode_SpecialEnd = kAVCode_Luck,

	kAVCode_ActionPoints = 0xC,
	kAVCode_CarryWeight = 0xD,
	kAVCode_CritChance = 0xE,
	kAVCode_HealRate = 0xF,
	kAVCode_Health = 0x10,
	kAVCode_MeleeDamage = 0x11,
	kAVCode_DamageResistance = 0x12,
	kAVCode_PoisonResist = 0x13,
	kAVCode_RadResist = 0x14,
	kAVCode_SpeedMult = 0x15,
	kAVCode_Fatigue = 0x16,
	kAVCode_Karma = 0x17,
	kAVCode_XP = 0x18,

	kAVCode_PerceptionCondition = 0x19,
	kAVCode_EnduranceCondition = 0x1A,
	kAVCode_LeftAttackCondition = 0x1B,
	kAVCode_RightAttackCondition = 0x1C,
	kAVCode_LeftMobilityCondition = 0x1D,
	kAVCode_RightMobilityCondition = 0x1E,
	kAVCode_BrainCondition = 0x1F,
	kAVCode_BodyPartStart = kAVCode_PerceptionCondition,
	kAVCode_BodyPartEnd = kAVCode_BrainCondition,

	kAVCode_Barter = 0x20,
	kAVCode_BigGuns = 0x21,
	kAVCode_EnergyWeapons = 0x22,
	kAVCode_Explosives = 0x23,
	kAVCode_Lockpick = 0x24,
	kAVCode_Medicine = 0x25,
	kAVCode_MeleeWeapons = 0x26,
	kAVCode_Repair = 0x27,
	kAVCode_Science = 0x28,
	kAVCode_Guns = 0x29,
	kAVCode_Sneak = 0x2A,
	kAVCode_Speech = 0x2B,
	kAVCode_Survival = 0x2C,
	kAVCode_Unarmed = 0x2D,
	kAVCode_SkillsStart = kAVCode_Barter,
	kAVCode_SkillsEnd = kAVCode_Unarmed,

	kAVCode_InventoryWeight = 0x2E,
	kAVCode_Paralysis = 0x2F,
	kAVCode_Invisibility = 0x30,
	kAVCode_Chameleon = 0x31,
	kAVCode_NightEye = 0x32,
	kAVCode_Turbo = 0x33,
	kAVCode_FireResist = 0x34,
	kAVCode_WaterBreathing = 0x35,
	kAVCode_RadiationRads = 0x36,
	kAVCode_BloodyMess = 0x37,
	kAVCode_UnarmedDamage = 0x38,
	kAVCode_Assistance = 0x39,
	kAVCode_ElectricResist = 0x3A,
	kAVCode_FrostResist = 0x3B,
	kAVCode_EnergyResist = 0x3C,
	kAVCode_EmpResist = 0x3D,

	kAVCode_Variable01 = 0x3E,
	kAVCode_Variable02 = 0x3F,
	kAVCode_Variable03 = 0x40,
	kAVCode_Variable04 = 0x41,
	kAVCode_Variable05 = 0x42,
	kAVCode_Variable06 = 0x43,
	kAVCode_Variable07 = 0x44,
	kAVCode_Variable08 = 0x45,
	kAVCode_Variable09 = 0x46,
	kAVCode_Variable10 = 0x47,

	kAVCode_IgnoreCrippledLimbs = 0x48,
	kAVCode_Dehydration = 0x49,
	kAVCode_Hunger = 0x4A,
	kAVCode_SleepDeprivation = 0x4B,
	kAVCode_DamageThreshold = 0x4C,
	kAVCode_Max = 0x4D,
};

enum LifeStates
{
	kLifeState_Alive = 0,
	kLifeState_Dying = 1,
	kLifeState_Dead = 2,
	kLifeState_Unconscious = 3,
	kLifeState_Reanimate = 4,
	kLifeState_Restrained = 5,
	kLifeState_EssentialUnconscious = 6,
};

class Actor : public MobileObject
{
public:

	Actor();
	~Actor();

	virtual void		GetByte1C1(void);
	virtual void		Unk_C2(void);
	virtual void		UpdateFlt1C4(void);
	virtual void		GetFlt1C4(void);
	virtual void		Unk_C5(void);
	virtual void		Unk_C6(void);
	virtual void		SetIgnoreCrime(void);
	virtual void		GetIgnoreCrime(void);
	virtual void		Resurrect(void);
	virtual void		Unk_CA(void);
	virtual void		Unk_CB(void);
	virtual void		Unk_CC(void);
	virtual void		GetMaxActorValues(void);
	virtual void		DamageHealthAndFatigue(void);
	virtual void		DamageActionPoints(void);
	virtual void		DamageActionPointsForAction(void);
	virtual void		GetDisposition(void);
	virtual void		UpdateMovement(void);
	virtual void		Unk_D3(void);
	virtual void		Unk_D4(void);
	virtual void		Unk_D5(void);
	virtual void		GetIsOverencumbered(void);
	virtual void		Unk_D7(void);
	virtual void		IsPlayerRef(void);	// IsPlayerRef
	virtual void		GetIsInCanibalAction(void);
	virtual void		Unk_DA(void);
	virtual void		GetIsInSandmanAction(void);
	virtual void		Unk_DC(void);
	virtual void		SandmanActor(void);
	virtual void		CannibalizeActor(void);
	virtual void		GetRace(void);
	virtual void		GetHandReachTimesCombatDistance(void);
	virtual void		Unk_E1();
	virtual void		Unk_E2(void);
	virtual void		HasRagdoll(void);
	virtual void		GetActorType(void);	// Creature = 0, Character = 1, PlayerCharacter = 2
	virtual void		SetActorValue(void);
	virtual void		SetActorValueInt(void);
	virtual void		Unk_E7(void);
	virtual void		Unk_E8(void);
	virtual void		ForceActorValue(void);
	virtual void		ModActorValue(void);
	virtual void		DamageActorValue(void);
	virtual void		Unk_EC(void);
	virtual void		Unk_ED(void);
	virtual void		Unk_EE(void);
	virtual void		GetPreferedWeapon(void);
	virtual void		Unk_F0(void);
	virtual void		ResetActorDTDR(void);
	virtual void		DamageItem(void);
	virtual void		DropItem(void);
	virtual void		HandlePickupItem(TESObjectREFR* activatedRef, UInt32 count, bool arg3);
	virtual void		Unk_F5(void);
	virtual void		CastWeaponEffect(void);
	virtual void		Unk_F7(void);
	virtual void		AddSpellToList(void);	// 0F8 bool AddSpell(spell)
	virtual void		Unk_F9(void);
	virtual void		Reload(TESObjectWEAP* weapon, UInt32 animtype, UInt8 hasExtendedClip);
	virtual void		ReloadAlt(TESObjectWEAP* weapon, UInt32 animtype, UInt8 hasExtendedClip, char isInstantSwapHotkey);
	virtual void		DecreaseClipAmmo(UInt32 count);
	virtual void		Unk_FD(void);
	virtual void		GetCombatGroup(void);
	virtual void		Unk_FF(void);
	virtual void		Unk_100(void);
	virtual void		Unk_101(void);
	virtual void		Unk_102(void);
	virtual void		Unk_103(void);
	virtual void		Unk_104(void);
	virtual void		Unk_105(void);
	virtual void		Unk_106(void);
	virtual void		UpdateFlt02B0(void);
	virtual void		GetAlpha(void);
	virtual void		ForceAttackActor(void);
	virtual bool		IsInCombat(void);
	virtual Actor*		GetCombatTarget(void);
	virtual void		UpdateCombat(void);
	virtual void		Unk_10D(void);
	virtual void		Unk_10E(void);
	virtual void		GetTotalArmorDR(void);
	virtual void		GetTotalArmorDT(void);
	virtual void		GetCreatureDamage(void);
	virtual void		IsTrespassing(void);
	virtual void		Unk_113(void);
	virtual void		SetWantsWeaponOut(void);
	virtual void		Unk_115(void);
	virtual void		CalculateSpeedMult(void);
	virtual void		CalculateRunSpeed(void);
	virtual void		Unk_118(void);
	virtual void		Unk_119(void);
	virtual void		Unk_11A(void);
	virtual void		Unk_11B(void);
	virtual void		GetAttacked(void);
	virtual void		Unk_11D(void);
	virtual void		Unk_11E(void);
	virtual void		nullsub_11F(void);
	virtual void		Unk_120(void);
	virtual void		Unk_121(void);
	virtual void		RewardXP(void);	//	0122	Check for LevelUp
	virtual void		Unk_123(void);
	virtual void		InternalSetActorValue(void);
	virtual void		Unk_125(void);
	virtual void		SetPerkRank(BGSPerk* perk, UInt8 rank, bool alt);
	virtual void		RemovePerk(void);
	virtual UInt8		GetPerkRank(BGSPerk* perk, bool alt);
	virtual void		Unk_129(void);
	virtual void		RemovePerkEntry(void);
	virtual void		GetPerkModifier(void);
	virtual void		Unk_12C(void);
	virtual void		IsImmobile(void);
	virtual void		DoHealthDamage(void);
	virtual void		Unk_12F(void);
	virtual void		Unk_130(void);
	virtual void		Unk_131(void);
	virtual void		HandleHeadTracking(void);
	virtual void		UpdateHeadTrackingEmotions(void);
	virtual void		CreateActorMover(void);
	virtual void		DestroyActorMover(void);
	virtual void		GetBaseActorValue(void);
	
	MagicCaster							magicCaster;					// 088
	MagicTarget							magicTarget;					// 094
	ActorValueOwner						avOwner;						// 0A4
	CachedValuesOwner					cvOwner;						// 0A8

	bhkRagdollController*				ragDollController;				// 0AC
	bhkRagdollPenetrationUtil*			ragDollPentrationUtil;			// 0B0
	UInt32								unk0B4;							// 0B4-
	float								flt0B8;							// 0B8
	UInt8								byte0BC;						// 0BC-
	UInt8								byte0BD;						// 0BD
	UInt8								byte0BE;						// 0BE
	UInt8								byte0BF;						// 0BF
	Actor*								killer;							// 0C0
	UInt8								byte0C4;						// 0C4-
	UInt8								byte0C5;						// 0C5
	UInt8								byte0C6;						// 0C6
	UInt8								byte0C7;						// 0C7
	float								flt0C8;							// 0C8
	float								flt0CC;							// 0CC
	TList<void>							list0D0;						// 0D0
	UInt8								byte0D8;						// 0D8
	UInt8								byte0D9;						// 0D9
	UInt8								byte0DA;						// 0DA
	UInt8								byte0DB;						// 0DB
	UInt32								unk0DC;							// 0DC
	TList<void>							list0E0;						// 0E0
	UInt8								byte0E8;						// 0E8	const 1
	UInt8								byte0E9;						// 0E9
	UInt8								byte0EA;						// 0EA
	UInt8								byte0EB;						// 0EB
	UInt32								unk0EC;							// 0EC
	UInt8								byte0F0;						// 0F0-
	UInt8								byte0F1;						// 0F1-
	UInt8								byte0F2;						// 0F2
	UInt8								byte0F3;						// 0F3
	TList<void>							list0F4;						// 0F4
	TList<void>							list0FC;						// 0FC
	UInt8								isInCombat;						// 104 Is in combat
	UInt8								jipActorFlags1;					// 105
	UInt8								jipActorFlags2;					// 106
	UInt8								jipActorFlags3;					// 107
	UInt32								lifeState;						// 108 saved as byte HasHealth = 1 or 2, optionally 6
	UInt32								criticalStage;					// 10C
	UInt32								animGroupId110;					// 110
	float								flt114;							// 114
	UInt8								byte118;						// 118-
	UInt8								byte119;						// 119+
	UInt16								jip11A;							// 11A+
	UInt32								unk11C;							// 11C-
	UInt32								unk120;							// 120-
	bool								forceRun;						// 124
	bool								forceSneak;						// 125
	UInt8								byte126;						// 126-
	UInt8								byte127;						// 127-
	Actor*								combatTarget;					// 128
	BSSimpleArray<Actor*>*				combatTargets;					// 12C
	BSSimpleArray<Actor*>*				combatAllies;					// 130
	UInt8								byte134;						// 134-
	UInt8								byte135;						// 135+
	UInt16								jip136;							// 136+
	UInt32								unk138;							// 138-
	UInt32								unk13C;							// 13C-
	UInt32								actorFlags;						// 140	0x80000000 - IsEssential
	bool								ignoreCrime;					// 144
	UInt8								byte145;						// 145	Has to do with package evaluation
	UInt8								byte146;						// 146	Has to do with package evaluation
	UInt8								byte147;						// 147
	UInt32								unk148;							// 148-
	UInt8								inWater;						// 14C
	UInt8								isSwimming;						// 14D
	UInt8								unk14E;							// 14E
	UInt8								unk14F;							// 14F
	UInt32								unk150;							// 150	-	if Actor and not player: 0160 receives a copy of Pos[X,Y,Z] and Rot if 0170 is null during Func011B. 0170 receive ParentCell if Interior, Worldspace otherwize
	UInt32								unk154;
	UInt32								unk158;
	UInt32								unk15C;
	UInt32								unk160;							// 160 if Actor and not player: 0160 receives a copy of Pos[X,Y,Z] and Rot if 0170 is null during Func011B
	UInt32								unk164;
	UInt32								unk168;
	UInt32								unk16C;
	UInt32								unk170;							// 170 receive ParentCell if Interior, Worldspace otherwize
	UInt32								unk174;
	Float32								spreadWeapon;
	Float32								spreadScope;					// 17C	 
	Float32								spreadActor;
	Float32								spreadVATS;
	Float32								spreadHealthPercent;			// 188
	UInt8								byte18C;						// 18C-
	bool								isTeammate;						// 18D
	UInt8								byte18E;						// 18E-
	UInt8								byte18F;						// 18F
	ActorMover*							actorMover;						// 190

	UInt32								unk194;							// 194-
	UInt32								unk198;							// 198-
	Float32								flt19C;							// 19C
	UInt32								unk1A0;							// 1A0-
	UInt32								unk1A4;							// 1A4-
	UInt32								unk1A8;							// 1A8-
	UInt32								unk1AC;							// 1AC-

	UInt8								unk1B0;							// 1B0
	UInt8								unk1B1;							// 1B1
	UInt8								unk1B2[2];						// 1B2

	void								FireWeapon();
	void								EjectFromWeapon(TESObjectWEAP* weapon);
	TESObjectWEAP*						GetWeaponForm() const;
	bool								IsAnimActionReload() const;
	TESActorBase*						GetActorBase();
	void								EquipItem(TESForm* objType, UInt32 equipCount = 1, ExtraDataList* itemExtraList = nullptr, UInt32 shouldApplyEnchantment = 1,
													UInt32 lockEquip = false, UInt32 noMessage = 1);
	void								UnequipItem(TESForm* objType, UInt32 unequipCount = 1, ExtraDataList* itemExtraList = nullptr, UInt32 shouldApplyEnchantment = 1,
													UInt32 lockUnequip = false, UInt32 noMessage = 1);

	std::vector<TESForm*>				GetEquippedItems();
	InventoryChangesArray				GetEquippedEntryDataList();
	Float64								GetCalculatedSpread(UInt32 mode = 0, InventoryChanges* entry = nullptr);

	bool								IsDoingAttackAnim() { return ThisCall<bool>(0x894900, this); }
	bool								IsCombatTarget(const Actor* source);
	bool								IsHostileCompassTarget() const;

	Float32								GetHitDataValue(UInt32 valueType) const;
	Float32								GetActorValue(ActorValueCode avcode);
	bool								GetLineOfSight(TESObjectREFR* actor) { return ThisCall<bool>(0x88B880, this, 0, actor, 1, 0, 0); }
	Float64								GetMaxCarryWeight() { return ThisCall<Float64>(0x8A0C20, this); }
	__forceinline void					PlayPickupPutdownSounds(TESForm* item, char isPickup, char isEquip) { ThisCall(0x8ADED0, this, item, isPickup, isEquip); }

	void								SendStealingAlarm(TESObjectREFR* container, bool checkItems);
};
static_assert(sizeof(Actor) == 0x1B4);

class Creature : public Actor
{
public:
	Creature();
	~Creature();

	virtual void	Unk_137(void);

	UInt32			unk1B4[3];			// 1B4
};
static_assert(sizeof(Creature) == 0x1C0);


class Character : public Actor
{
public:
	Character();
	~Character();

	virtual void	Unk_137(void);
	virtual void	nullsub_138(void);

	ValidBip01Names*	validBip01Names;	// 1B4
	float				totalArmorDR;		// 1B8
	float				totalArmorDT;		// 1BC
	UInt8				isTrespassing;		// 1C0
	UInt8				byt1C1;				// 1C1
	UInt16				unk1C2;				// 1C2
	float				unk1C4;				// 1C4
};
static_assert(sizeof(Character) == 0x1C8);

typedef TList<BGSQuestObjective::Target> QuestObjectiveTargets;

struct PerkRank
{
	BGSPerk*	perk;
	UInt8		rank;
	UInt8		pad05[3];
};
struct CasinoStats
{
	UInt32		casinoRefID;
	UInt32		earnings;
	UInt16		unk08;
	UInt8		gap0A[2];
};

// 9BC
class PlayerCharacter : public Character
{
public:
	PlayerCharacter();
	~PlayerCharacter();

	// used to flag controls as disabled in disabledControlFlags
	enum {
		kControlFlag_Movement		= 1 << 0,
		kControlFlag_Look			= 1 << 1,
		kControlFlag_Pipboy			= 1 << 2,
		kControlFlag_Fight			= 1 << 3,
		kControlFlag_POVSwitch		= 1 << 4,
		kControlFlag_RolloverText	= 1 << 5,
		kControlFlag_Sneak			= 1 << 6,
	};

	virtual void		ReturnFalse(void);
	virtual void		GetPerkRanks(void);

	struct MapMarkerInfo
	{
		ExtraMapMarkerData*	markerData;
		TESObjectREFR*		markerRef;
	};

	struct CompassTarget
	{
		Actor*		target;
		UInt8		isHostile;
		UInt8		isDetected;
		UInt8		pad06[2];
	};

	// lotsa data

	UInt32								unk1C8[17];				// 1C8	208 could be a DialogPackage
	void*								unk20C;					// 20C
	TList<ActiveEffect>*				activeEffects;			// 210
	UInt32								unk214[2];				// 214
	void*								unk21C;					// 21C
	UInt32								unk220[8];				// 220	224 is a package of type 1C
	bool								showQuestItems;			// 240
	UInt8								byte241;				// 241
	UInt8								byte242;				// 242
	UInt8								byte243;				// 243
	float								unk244[77];				// 244	have to be a set of ActorValue
	float								permAVMods[77];			// 378	have to be a set of ActorValue
	float								flt4AC;					// 4AC
	float								actorValues4B0[77];		// 4B0	have to be a set of ActorValue
	BGSNote*							note;					// 5E4
	UInt32								unk574;					// 5E8
	ImageSpaceModifierInstanceDOF*		unk5EC;					// 5EC
	ImageSpaceModifierInstanceDOF*		unk5F0;					// 5F0
	ImageSpaceModifierInstanceDRB*		unk5F4;					// 5F4
	UInt32								unk5F8;					// 5F8
	TList<Actor>						teammates;				// 5FC
	TESObjectREFR*						lastExteriorDoor;		// 604
	UInt8								isPlayingAttackSound;	// 608
	UInt8								pad609[3];				// 609
	TList<void>*						actionList;				// 60C
	TList<void>*						casinoDataList;			// 610
	TList<void>*						caravanCards1;			// 614
	TList<void>*						caravanCards2;			// 618
	UInt32								unk61C[6];				// 61C
	void*								ptr634;					// 634	bhkMouseSpringAction when there's a grabbed reference
	TESObjectREFR*						grabbedRef;				// 638
	UInt32								unk63C;					// 63C
	UInt32								unk640;					// 640
	float								grabbedWeight;			// 644
	bool								thirdPersonCacheValue;	// 648
	UInt8								unk649;					// 649
	bool								isInThirdPerson;		// 64A	= not FirstPerson -- Gets weirdly changed as a hack and thirdPersonCacheValue stores real value
	UInt8								unk64B;					// 64B
	bool								thirdPersonConsistent;	// 64C
	UInt8								byte64D;				// 64D
	UInt8								byte64E;				// 64E
	bool								isUsingScope;			// 64F
	UInt8								byte650;				// 650
	bool								alwaysRun;				// 651
	bool								autoMove;				// 652
	UInt8								byte653;				// 653
	UInt32								sleepHours;				// 654
	UInt8								isResting;				// 658	Something to do with SleepDeprivation; see 0x969DCF
	UInt8								byte659;				// 659
	UInt8								byte65A;				// 65A
	UInt8								byte65B;				// 65B
	UInt32								unk65C[4];				// 65C
	UInt8								byte66C;				// 66C
	UInt8								byte66D;				// 66D
	UInt8								byte66E;				// 66E
	UInt8								byte66F;				// 66F
	float								worldFOV;				// 670
	float								firstPersonFOV;			// 674
	float								flt678;					// 678
	UInt32								unk67C;					// 67C
	UInt8								disabledControlFlags;	// 680 kControlFlag_xxx
	UInt8								unk0681[3];				// 681
	UInt32								unk684[2];				// 684
	ValidBip01Names*					bipedAnims1st;			// 68C
	AnimData*							firstPersonAnimData;	// 690 ExtraDataAnim::Data
	NiNode*								node1stPerson;			// 694 used as node if unk64A is true
	UInt32								unk698;					// 698
	UInt32								unk69C;					// 69C
	AnimData*							unkNotTpsAniMData;		// 6A0
	UInt32								unk6A4;					// 6A4
	TList<TESTopic>						topicList;				// 6A8
	UInt32								unk6AC[2];				// 6AC
	TESQuest*							activeQuest;			// 6B8
	TList<BGSQuestObjective>			questObjectiveList;		// 6BC
	UInt32								unk6C4[39];				// 6C4
	TESRegion*							currentRegion;			// 760
	TESRegionList						regionsList;			// 764
	UInt32								unk770[18];				// 770
	UInt8								gameDifficulty;			// 7B8
	UInt8								pad7B9[3];				// 7B9
	bool								isHardcore;				// 7BC
	UInt8								pad7BD[3];				// 7BD
	UInt32								unk7C0[49];				// 7C0
	TList<BGSEntryPointPerkEntry>		perkEntries[74];		// 884
	TList<void>							perkRanksTM;			// AD4
	TList<BGSEntryPointPerkEntry>		perkEntriesTM[74];		// ADC
	UInt32								unkD2C[4];				// D2C
	NiObject*							unkD3C;					// D3C
	UInt32								unkD40;					// D40
	Actor*								reticleActor;			// D44
	TList<CompassTarget>*				compassTargets;			// D48
	UInt32								unkD4C;					// D4C
	float								lastAmmoSwapTime;		// D50
	UInt32								unkD54[4];				// D4C
	CombatActors*						combatActors;			// D64
	UInt32								teammateCount;			// D68
	UInt32								unkD6C[5];				// D6C
	NiNode*								niNodeD80;				// D80
	UInt32								unkD84[12];				// D84
	NiNode*								niNodeDB4;				// DB4
	UInt32								unkDB8[7];				// DB8
	NiPoint3							vectorDD4;				// DD4
	NiPoint3							cameraPos;				// DE0
	bhkRigidBody*						rigidBody;				// DEC
	bool								pcInCombat;				// DF0
	bool								pcUnseen;				// DF1
	UInt8								byteDF2;				// DF2
	UInt8								byteDF3;				// DF3
	BSSimpleArray<void>					itemChanges;			// DF4
	UInt32								unkE04[5];				// E04
	float								killCamTimer;			// E18
	float								killCamCooldown;		// E1C
	UInt8								byteE20;				// E20
	UInt8								byteE21;				// E21
	UInt8								byteE22;				// E22
	UInt8								byteE23;				// E23
	float								fltE24;					// E24
	float								counterAttackTimer;		// E28
	UInt32								unkE2C;					// E2C
	UInt32								itemDetectionTimer;		// E30
	NiNode*								ironSightNode;			// E34
	bool								noHardcoreTracking;		// E38	Appears to be unused
	bool								skipHCNeedsUpdate;		// E39
	UInt8								byteE3A;				// E3A
	UInt8								byteE3B;				// E3B
	BSSimpleArray<TESAmmo*>				hotkeyedWeaponAmmos;	// E3C
	UInt32								unkE4C;					// E4C

		// 7C4 is a byte used during Combat evaluation (Player is targetted ?), 
		// 7C6 is a boolean meaning toddler, 
		// 7C7 byte bool PCCanUsePowerArmor, Byt0E39 referenced during LoadGame
		// Used when entering FlyCam : 7E8/7EC/7F0 stores Pos, 7F0 adjusted by scaledHeight multiplied by 0698 , 7E0 stores RotZ, 7E4 RotX
		// Perks forms a list at 87C and AD4. Caravan Cards at 614 and 618. Quest Stage LogEntry at 6B0. Quest Objectives at 6BC.
		// Hardcore flag would be E38. Byte at DF0 seems to be PlayerIsInCombat
		// TList at 6C4 is cleared when there is no current quest. There is another NiNode at 069C
		// list of perk and perkRank are at 0x087C and 0x0AD4 (alt perks). 086C is cleared after equipement change.
		// D68 counts the Teammates.
		// D74: 96 bytes that are cleared when the 3D is cleared.

	bool					IsThirdPerson() { return thirdPersonConsistent; }
	UInt32					GetMovementFlags() { return actorMover->GetMovementFlags(); }	// 11: IsSwimming, 9: IsSneaking, 8: IsRunning, 7: IsWalking, 0: keep moving
	bool					IsPlayerSwimming() { return GetMovementFlags()  >> 11 & 1; }

	__forceinline static PlayerCharacter*	GetSingleton() { return *reinterpret_cast<PlayerCharacter**>(0x011DEA3C); };
	bool					SetSkeletonPath(const char* newPath);
	bool					SetSkeletonPath_v1c(const char* newPath);	// Less worse version as used by some modders
	static void				UpdateHead();
	QuestObjectiveTargets*	GetCurrentQuestObjectiveTargets();
	TESObjectREFR*			GetPlacedMarkerOrTeleportLink();
	HighProcess*			GetHighProcess() { return reinterpret_cast<HighProcess*>(baseProcess); };
	__forceinline bool		UsingIronSights() { return this->ironSightNode && this->baseProcess->IsWeaponOut() || this->baseProcess->IsAiming(); }
};
static_assert(sizeof(PlayerCharacter) == 0xE50);

class NonActorMagicCaster : public BSExtraData
{
public:
	MagicCaster		caster;		// 0C
};

class Explosion : public MobileObject
{
public:
	Explosion();
	~Explosion();

	UInt32				unk088;
	Float32				unk08C;		// init to 3.0
	Float32				buildTime;
	Float32				radius;
	Float32				ISRadius;
	UInt32				unk09C;
	TESObjectREFR*		object0A0;
	TList<void>			list0A4;
	Sound				sound0AC;
	Sound				sound0B8;
	void*				pointLight;
	TESObjectREFR*		source;
	union
	{
		TESObjectREFR*	object0CC;			// unused most of the time
	};
	TESObjectREFR*		object0D0;
	UInt32				unk0D4;
	UInt8				dontApplyExplosionIMOD;
	UInt8				isUnderwater;
	UInt8				byte0DA;
	UInt8				byte0DB;
	union										// 0DC
	{
		TESAmmo*				ammo;							// unused for actors ???
		NonActorMagicCaster*	extraCaster;		
	};
	TESObjectWEAP*		weapon;			// 0E0
	UInt32				unk0E4;			// 0E4
	NiPoint3			vec0E8;			// 0E8
	NiPoint3			vec0F4;			// 0F4
	Float32				unk100;			// init to -1

	bool CanStoreAmmo();
};
static_assert(sizeof(Explosion) == 0x104);


class Projectile : public MobileObject
{
public:
	Projectile();
	~Projectile();

	TList<BGSImpactData>	impactDataList;		// 0x088
	UInt8					hasImpacted;		// 0x08C
	UInt8					pad091[3];
	NiMatrix33				nimatrix33;			// 0x094
	NiPoint3				nip3;				// 0x0B8	saved separately
	UInt32					unk0C4;				// 0x0C4	saved separately
	UInt32					projFlags;			// 0x0C8
	Float32					speedMult1;			// 0x0CC
	Float32					speedMult2;			// 0x0D0
	Float32					range;				// 0x0D4
	Float32					elapsedTime;		// 0x0D8
	Float32					hitDamage;			// 0x0DC
	Float32					fadeTimePercent;	// 0x0E0
	Float32					detonationTime;		// 0x0E4
	Float32					mineBlinkTimer;		// 0x0E8
	Float32					spreadz;			// 0x0EC
	Float32					spreadx;			// 0x0F0
	union										// 0x0F4
	{
		TESAmmo*			ammo;				
		Float32				weaponHealthPercent;// set but unused
	};
	TESObjectWEAP*			weapon;				// 0x0F8
	TESObjectREFR*			source;				// 0x0FC
	TESObjectREFR*			target;				// 0x100 live grenade target
	NiPoint3				pt104;				// 0x104
	UInt32					distTravelled;		// 0x110
	void*					light;				// 0x114
	UInt8					byte118;			// 0x118
	UInt8					pad119[3];			// 0x119
	void*					node11C;			// 0x11C
	UInt32					unk120;				// 0x120
	Float32					flt124;				// 0x124
	Sound					passPlayerSound;	// 0x128
	Sound					countDownSound;		// 0x134
	UInt32					unk140;				// 0x140
	InventoryChanges*		RILContChange;		// 0x144
	UInt8					hasPlayedPassPlayerSound;// 0x148
	UInt8					pad149[3];			// 0x149
	Float32					maxrange;
};
static_assert(sizeof(Projectile) == 0x150);

struct ParentSpaceNode;
struct TeleportLink
{
	TESObjectREFR*	door;
	UInt32			unk04[3];
};

struct QuestTarget
{
	BSSimpleArray<ParentSpaceNode*>		parentSpaceNodes;
	BSSimpleArray<TeleportLink*>		teleportLinks;
	NiPoint3							startPos;
	NiPoint3							endPos;
};
static_assert(sizeof(QuestTarget) == 0x38);

template <typename ...Args>
void ApplyPerkModifiers(UInt32 id, Actor* actor, Args ...args)
{
	CdeclCall<void>(0x05E58F0, id, actor, std::forward<Args>(args)...);
}

// 64
struct ActorHitData
{
	enum HitFlags
	{
		kFlag_TargetIsBlocking = 1 << 0,
		kFlag_TargetWeaponOut = 1 << 1,
		kFlag_IsCritical = 1 << 2,
		kFlag_OnDeathCritEffect = 1 << 3,
		kFlag_IsFatal = 1 << 4,
		kFlag_DismemberLimb = 1 << 5,
		kFlag_ExplodeLimb = 1 << 6,
		kFlag_CrippleLimb = 1 << 7,
		kFlag_BreakWeaponNonEmbedded = 1 << 8,
		kFlag_BreakWeaponEmbedded = 1 << 9,
		kFlag_IsSneakAttack = 1 << 10,
		kFlag_ArmorPenetrated = 0x80000000	// JIP only
	};

	Actor*				source;			// 00
	Actor*				target;			// 04
	union								// 08
	{
		Projectile*		projectile;
		Explosion*		explosion;
		Actor*			actor;
	};
	UInt32				weaponAV;		// 0C
	SInt32				hitLocation;	// 10
	Float32				healthDmg;		// 14
	Float32				wpnBaseDmg;		// 18	Skill and weapon condition modifiers included
	Float32				fatigueDmg;		// 1C
	Float32				limbDmg;		// 20
	Float32				blockDTMod;		// 24
	Float32				armorDmg;		// 28
	Float32				weaponDmg;		// 2C
	TESObjectWEAP*		weapon;			// 30
	Float32				healthPerc;		// 34
	NiPoint3			impactPos;		// 38
	NiPoint3			impactAngle;	// 44
	UInt32				unk50;			// 50
	void*				ptr54;			// 54
	UInt32				flags;			// 58
	Float32				dmgMult;		// 5C
	SInt32				unk60;			// 60	Unused; rigged by CopyHitDataHook to store hitLocation

	TESAmmo* GetAmmo() const;
	TESObjectWEAP* GetWeapon() const;
	Script* GetAmmoScript() const;
};

enum APCostActions
{
	kActionPointsAttackUnarmed = 0x0,
	kActionPointsAttackOneHandMelee = 0x1,
	kActionPointsAttackTwoHandMelee = 0x2,
	kActionPointsAttackPistol = 0x3,
	kActionPointsAttackRifle = 0x4,
	kActionPointsAttackHandle = 0x5,
	kActionPointsAttackLauncher = 0x6,
	kActionPointsAttackGrenade = 0x7,
	kActionPointsAttackMine = 0x8,
	kActionPointsReload = 0x9,
	kActionPointsCrouch = 0xA,
	kActionPointsStand = 0xB,
	kActionPointsSwitchWeapon = 0xC,
	kActionPointsToggleWeaponDrawn = 0xD,
	kActionPointsHeal = 0xE,
	kActionPointsVATSUnarmedAttack1 = 0x11,
	kActionPointsOneHandThrown = 0x13,
	kActionPointsAttackThrown = 0x14,
	kActionPointsVATSUnarmedAttackGround = 0x15,
};

struct __declspec(align(4)) VATSQueuedAction
{
	APCostActions			actionType;
	UInt8					isSuccess;
	UInt8					byte05;
	UInt8					isMysteriousStrangerVisit;
	UInt8					byte07;
	UInt8					remainingShotsToFire_Burst;
	UInt8					count09;
	UInt8					gap0A[2];
	TESObjectREFR*			unkref;
	ActorValueCode			bodyPart;
	ActorHitData*			hitData;
	float					unk18;
	float					unk1C;
	float					apCost;
	UInt8					isMissFortuneVisit;
	UInt8					gap25[3];
};

enum AnimMoveTypes
{
	kAnimMoveType_Walking = 0x0,
	kAnimMoveType_Sneaking = 0x1,
	kAnimMoveType_Swimming = 0x2,
	kAnimMoveType_Flying = 0x3,
};

const char* GetActorValueString(UInt32 actorValue); // should work now
UInt32 GetActorValueForString(const char* strActorVal, bool bForScript = false);

typedef char* (*_GetActorValueName)(UInt32 actorValueCode);
extern const _GetActorValueName GetActorValueName;
UInt32 GetActorValueMax(UInt32 actorValueCode);
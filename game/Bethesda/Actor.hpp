#pragma once
#include "MobileObject.hpp"
#include "ActorValueOwner.hpp"
#include "CachedValuesOwner.hpp"
#include "MagicCaster.hpp"
#include "MagicTarget.hpp"
#include "ActorMover.hpp"
#include "ExtraContainerChanges.hpp"
#include "ActorValue.hpp"
#include "TESObjectWEAP.hpp"

class BGSPerk;
class bhkRagdollController;
class bhkRagdollPenetrationUtil;
class TESActorBase;
class ContinuousBeamProjectile;
class TESPackage;

class Actor :
	public MobileObject,		// 000
	public MagicCaster,			// 088
	public MagicTarget,			// 094
	public ActorValueOwner,		// 0A4
	public CachedValuesOwner	// 0A8
{
public:
	Actor();
	~Actor() override;
	virtual void		GetByte1C1();
	virtual void		Unk_C2();
	virtual void		UpdateFlt1C4();
	virtual void		GetFlt1C4();
	virtual void		Unk_C5();
	virtual void		Unk_C6();
	virtual void		SetIgnoreCrime();
	virtual void		GetIgnoreCrime();
	virtual void		Resurrect();
	virtual void		Unk_CA();
	virtual void		Unk_CB();
	virtual void		Unk_CC();
	virtual void		GetMaxActorValues();
	virtual void		DamageHealthAndFatigue();
	virtual void		DamageActionPoints();
	virtual void		DamageActionPointsForAction();
	virtual void		GetDisposition();
	virtual void		UpdateMovement();
	virtual void		Unk_D3();
	virtual void		Unk_D4();
	virtual void		Unk_D5();
	virtual void		GetIsOverencumbered();
	virtual void		Unk_D7();
	virtual void		IsPlayerRef();	// IsPlayerRef
	virtual void		GetIsInCanibalAction();
	virtual void		Unk_DA();
	virtual void		GetIsInSandmanAction();
	virtual void		Unk_DC();
	virtual void		SandmanActor();
	virtual void		CannibalizeActor();
	virtual void		GetRace();
	virtual void		GetHandReachTimesCombatDistance();
	virtual void		SetRefraction(bool, float);
	virtual void		Unk_E2();
	virtual void		HasRagdoll();
	virtual void		GetActorType();	// Creature = 0, Character = 1, PlayerCharacter = 2
	virtual void		SetActorValue();
	virtual void		SetActorValueInt();
	virtual void		Unk_E7();
	virtual void		Unk_E8();
	virtual void		ForceActorValue();
	virtual void		ModActorValue();
	virtual void		DamageActorValue();
	virtual void		Unk_EC();
	virtual void		Unk_ED();
	virtual void		Unk_EE();
	virtual void		GetPreferedWeapon();
	virtual void		Unk_F0();
	virtual void		ResetActorDTDR();
	virtual void		DamageItem();
	virtual void		DropItem();
	virtual void		HandlePickupItem(TESObjectREFR* apkActivatedRef, UInt32 auiCount, bool abUnk);
	virtual void		Unk_F5();
	virtual void		CastWeaponEffect();
	virtual void		Unk_F7();
	virtual void		AddSpellToList();	// 0F8 bool AddSpell(spell)
	virtual void		Unk_F9();
	virtual void		Reload(TESObjectWEAP* apkWeapon, TESObjectWEAP::WeaponReloadModes aeAnimtype, bool abHasExtendedClip);
	virtual void		ReloadAlt(TESObjectWEAP* apkWeapon, TESObjectWEAP::WeaponReloadModes aeAnimtype, bool abHasExtendedClip, bool abIsInstantSwapHotkey);
	virtual void		DecreaseClipAmmo(UInt32 auiCount);
	virtual void		Unk_FD();
	virtual void		GetCombatGroup();
	virtual void		Unk_FF();
	virtual void		Unk_100();
	virtual void		Unk_101();
	virtual void		Unk_102();
	virtual void		Unk_103();
	virtual void		Unk_104();
	virtual void		Unk_105();
	virtual void		Unk_106();
	virtual void		UpdateFlt02B0();
	virtual void		GetAlpha();
	virtual void		ForceAttackActor();
	virtual bool		IsInCombat();
	virtual Actor*		GetCombatTarget();
	virtual void		UpdateCombat();
	virtual void		Unk_10D();
	virtual void		Unk_10E();
	virtual void		GetTotalArmorDR();
	virtual void		GetTotalArmorDT();
	virtual void		GetCreatureDamage();
	virtual void		IsTrespassing();
	virtual void		Unk_113();
	virtual void		SetWantsWeaponOut();
	virtual void		Unk_115();
	virtual void		CalculateSpeedMult();
	virtual void		CalculateRunSpeed();
	virtual void		Unk_118();
	virtual void		Unk_119();
	virtual void		Unk_11A();
	virtual void		Unk_11B();
	virtual void		GetAttacked();
	virtual void		Unk_11D();
	virtual void		Unk_11E();
	virtual void		nullsub_11F();
	virtual void		Unk_120();
	virtual void		Unk_121();
	virtual void		RewardXP();	//	0122	Check for LevelUp
	virtual void		Unk_123();
	virtual void		InternalSetActorValue();
	virtual void		Unk_125();
	virtual void		SetPerkRank(BGSPerk* apkPerk, UInt8 aucRank, bool abAlt);
	virtual void		RemovePerk();
	virtual UInt8		GetPerkRank(BGSPerk* apkPerk, bool abAlt);
	virtual void		Unk_129();
	virtual void		RemovePerkEntry();
	virtual void		GetPerkModifier();
	virtual void		Unk_12C();
	virtual void		IsImmobile();
	virtual void		DoHealthDamage();
	virtual void		Unk_12F();
	virtual void		Unk_130();
	virtual void		Unk_131();
	virtual void		HandleHeadTracking();
	virtual void		UpdateHeadTrackingEmotions();
	virtual void		CreateActorMover();
	virtual void		DestroyActorMover();
	virtual void		GetBaseActorValue();

	enum LifeStates : UInt32
	{
		kLifeState_Alive					= 0x0,
		kLifeState_Dying					= 0x1,
		kLifeState_Dead						= 0x2,
		kLifeState_Unconscious				= 0x3,
		kLifeState_Reanimate				= 0x4,
		kLifeState_Restrained				= 0x5,
		kLifeState_EssentialUnconscious		= 0x6,
	};

	enum CriticalStages : UInt32
	{
		kCriticalStage_None					= 0x0,
		kCriticalStage_GooStart				= 0x1,
		kCriticalStage_GooEnd				= 0x2,
		kCriticalStage_DisintegrateStart	= 0x3,
		kCriticalStage_DisintegrateEnd		= 0x4,
	};

	struct Disposition
	{
		UInt32 disposition;
		UInt32 target;
	};

	struct ActorValueList : BSSimpleList<UInt32> {
		bool	unk;
		UInt32	unk2;
	};

	bhkRagdollController*		pkRagDollController;
	bhkRagdollPenetrationUtil*	pkRagDollPentrationUtil;
	UInt32						unk0B4;
	Float32						flt0B8;
	bool						bIsAIProcessing;
	UInt8						byte0BD;
	UInt8						byte0BE;
	UInt8						byte0BF;
	Actor*						pkKiller;
	UInt8						byte0C4;
	UInt8						byte0C5;
	UInt8						byte0C6;
	UInt8						byte0C7;
	Float32						time0C8;
	Float32						fCheckDeadBodyTimer;
	ActorValueList				kForceAVList;
	ActorValueList				kSetAVList;
	UInt8						byte0F0;
	UInt8						byte0F1;
	UInt8						byte0F2;
	UInt8						byte0F3;
	BSSimpleList<UInt32>		list0F4;
	BSSimpleList<Disposition*>	kDispositionList;
	bool						bIsInCombat;
	UInt8						jipActorFlags1;
	UInt8						jipActorFlags2;
	UInt8						jipActorFlags3;
	LifeStates					eLifeState;
	CriticalStages				eCriticalStage;
	UInt32						queuedAttackAnim;
	Float32						fPainSoundTimer;
	UInt8						byte118;
	UInt8						byte119;
	UInt16						jip11A;
	UInt32						flags11C;
	UInt32						hoursPassed120;
	bool						bForceRun;
	bool						bForceSneak;
	UInt8						byte126;
	UInt8						hasSearch;
	Actor*						pkCombatTarget;
	BSSimpleArray<Actor*>*		pkCombatTargets;
	BSSimpleArray<Actor*>*		pkCombatAllies;
	UInt8						byte134;
	UInt8						byte135;
	UInt16						jip136;
	UInt32						unk138;
	UInt32						uiMinorCrimeCount;
	UInt32						uiActorFlags;
	bool						bIgnoreCrime;
	UInt8						byte145;
	UInt8						bIsResetAI;
	UInt8						byte147;
	TESActorBase*				actorbase148;
	UInt8						bInWater;
	UInt8						bIsSwimming;
	UInt8						byte14E;
	UInt8						gap14F;
	UInt32						uiActionValue;
	Float32						fTimerOnAction;
	Float32						fUpdateTargetTimer;
	UInt8						byte15C;
	UInt8						bShouldRotateToTrack;
	UInt16						jip15E;
	NiPoint3					kStartingPos;
	Float32						flt16C;
	TESForm*					pkStartingWorldOrCell;
	UInt8						byte174;
	UInt8						byte175;
	UInt16						jip176;
	Float32						fCachedSpreadProjectile;
	Float32						fCachedSpreadScoped;
	Float32						fCachedSpreadWeaponModel;
	Float32						fCachedSpreadVATS;
	Float32						fCachedSpreadHealthPercent;
	UInt8						bIsCameraRagdollAnimDist;
	bool						bIsTeammate;
	UInt8						byte18E;
	UInt8						byte18F;
	ActorMover*					pkActorMover;
	UInt32						unk194;
	TESPackage*					package198;
	Float32						fRadiationDamageMultiplier;	// 1 - RadResist%
	ContinuousBeamProjectile*	continuousBeamProjectile;
	UInt32						uiSpeechExpression;
	UInt32						uiEmotionValue;
	UInt32						uiSitSleepState;
	UInt8						bIsTurret;
	bool						bForceHit;
	UInt8						byte1B2;
	UInt8						byte1B3;
	
	// OBSE: unk1 looks like quantity, usu. 1; ignored for ammo (equips entire stack). In NVSE, pretty much always forced internally to 1
	// OBSE: itemExtraList is NULL as the container changes entry is not resolved before the call
	// NVSE: Default values are those used by the vanilla script functions.
	void EquipItem(TESForm* objType, UInt32 equipCount = 1, ExtraDataList* itemExtraList = NULL, UInt32 unk3 = 1, bool lockEquip = false, UInt32 unk5 = 1);	// unk3 apply enchantment on player differently
	void UnequipItem(TESForm* objType, UInt32 unequipCount = 1, ExtraDataList* itemExtraList = NULL, UInt32 unk3 = 1, bool lockEquip = false, UInt32 unk5 = 1);
	bool IsInReloadAnim();
	bool IsWeaponOut();
	bool IsAiming();
	bool HasExtraRefractionProperty();
	bool CastsShadows();

	void								FireWeapon();
	void								EjectFromWeapon(TESObjectWEAP* weapon);
	TESObjectWEAP*						GetWeaponForm() const;
	bool								IsAnimActionReload() const;
	TESActorBase*						GetActorBase();
	void								EquipItem(TESForm* objType, UInt32 equipCount = 1, ExtraDataList* itemExtraList = nullptr, UInt32 shouldApplyEnchantment = 1,
		UInt32 lockEquip = false, UInt32 noMessage = 1);
	void								UnequipItem(TESForm* objType, UInt32 unequipCount = 1, ExtraDataList* itemExtraList = nullptr, UInt32 shouldApplyEnchantment = 1,
		UInt32 lockUnequip = false, UInt32 noMessage = 1);

	Float64								GetCalculatedSpread(UInt32 mode = 0, InventoryChanges* entry = nullptr);

	bool								IsDoingAttackAnim() { return ThisCall<bool>(0x894900, this); }
	bool								IsCombatTarget(const Actor* source);
	bool								IsHostileCompassTarget() const;

	Float32								GetHitDataValue(UInt32 valueType) const;
	Float32								GetActorValue(EnumActorValueCode avcode);
	bool								GetLineOfSight(TESObjectREFR* actor) { return ThisCall<bool>(0x88B880, this, 0, actor, 1, 0, 0); }
	Float64								GetMaxCarryWeight() { return ThisCall<Float64>(0x8A0C20, this); }
	__forceinline void					PlayPickupPutdownSounds(TESForm* item, char isPickup, char isEquip) { ThisCall(0x8ADED0, this, item, isPickup, isEquip); }

	void								SendStealingAlarm(TESObjectREFR* container, bool checkItems);
};
static_assert(sizeof(Actor) == 0x1B4);
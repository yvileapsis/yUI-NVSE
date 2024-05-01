#pragma once
#include "ActorPackage.hpp"

class Actor;
class ActorHitData;
struct AnimData;

class bhkCharacterController;

class BSFaceGenAnimationData;
class BSFaceGenNiNode;
class BSAnimGroupSequence;
class BSBound;

class DetectionData;

class TESAmmo;
class TESIdleForm;
class TESObjectREFR;
class TESObjectWEAP;

class TESPackage;
class TESPackageData;

class ItemChange;

class NiTriShape;

// 0x30
class BaseProcess 
{
public:
	BaseProcess();

	enum EnumQueuedIdleFlags
	{
		kIdleFlag_FireWeapon						= 1 << 0,
		kIdleFlag_Reload							= 1 << 1,
		kIdleFlag_CrippledLimb						= 1 << 4,
		kIdleFlag_Death								= 1 << 5,
		kIdleFlag_ForcedIdle						= 1 << 7,
		kIdleFlag_HandGrip							= 1 << 8,
		kIdleFlag_Activate							= 1 << 10,
		kIdleFlag_StandingLayingDownChange			= 1 << 11,
		kIdleFlag_EquipOrUnequip					= 1 << 14,
		kIdleFlag_AimWeapon							= 1 << 16,
		kIdleFlag_AttackEjectEaseInFollowThrough	= 1 << 17,
		kIdleFlag_SomethingAnimatingReloadLoop		= 1 << 18,
	};

	struct CachedValues
	{
		enum EnumFlags
		{
			kCached_Radius = 0x1,
			kCached_WidthX = 0x2,
			kCached_WidthY = 0x4,
			kCached_DPS = 0x8,
			kCached_MedicineSkillMult = 0x10,
			kCached_Paralysis = 0x20,
			kCached_HealRate = 0x40,
			kCached_FatigueReturnRate = 0x80,
			kCached_PerceptionCondition = 0x100,
			kCached_EyeHeight = 0x200,
			kCached_SomethingShouldAttack = 0x400,
			kCached_WalkSpeed = 0x1000,
			kCached_RunSpeed = 0x2000,
			kCached_NoCrippledLegs = 0x4000,
			kCached_Height = 0x8000,
			kCached_IsGhost = 0x10000000,
			kCached_Health = 0x20000000,
			kCached_Fatigue = 0x40000000,
			kCached_SurvivalSkillMult = 0x80000000
		};

		Float32		fRadius;
		Float32		fWidthX;
		Float32		fWidthY;
		Float32		fHeight;
		Float32		fDamagePerSecond;
		Float32		fMedicineSkillMult;
		Float32		fSurvivalSkillMult;
		Float32		fParalysis;
		Float32		fHealRate;
		Float32		fFatigueReturnRate;
		Float32		fPerceptionCondition;
		Float32		fEyeHeight;
		SInt32		unk30;
		SInt32		unk34;
		Float32		fWalkSpeed;
		Float32		fRunSpeedMult;
		UInt8		bHasNoCrippledLegs;
		UInt8		pad41[3];
		EnumFlags	eFlags;
	};

	enum EnumProcessLevel : UInt32
	{
		kProcessLevel_High = 0,
		kProcessLevel_MiddleHigh = 1,
		kProcessLevel_MiddleLow = 2,
		kProcessLevel_Low = 3
	};

	virtual							~BaseProcess();
	virtual void					Copy();
	virtual void					CopyPackage();
	virtual void					Unk_03();
	virtual void					Update();
	virtual void					UpdateAlt();
	virtual void					Unk_06();
	virtual void					Unk_07();
	virtual void					Unk_08();
	virtual void					CheckForNewPackage();
	virtual void					SetLastTimeProcessed();
	virtual void					Unk_0B();
	virtual void					Unk_0C();
	virtual void					Unk_0D();
	virtual void					Unk_0E();
	virtual void					Unk_0F();
	virtual void					GetUnk0B4();
	virtual void					FindSpecialIdletoPlay();
	virtual void					Unk_12();
	virtual void					Unk_13();
	virtual void					Unk_14();
	virtual void					Unk_15();
	virtual void					Unk_16();
	virtual void					Unk_17();
	virtual void					GetIdleDoneOnce();
	virtual void					Unk_19();
	virtual void					Unk_1A();
	virtual void					Unk_1B();
	virtual void					Unk_1C();
	virtual void					Unk_1D();
	virtual void					Unk_1E();
	virtual void					ProcessSandman();
	virtual void					ProcessCannibal();
	virtual void					Unk_21();
	virtual void					SetDoingSayTo();
	virtual void					SetTargetForPackage();
	virtual void					SetLocationForPackage();
	virtual void					Unk_25();
	virtual void					GetCurrentDestinationCoordinate();
	virtual void					GetCurrentDestinationCell();
	virtual void					GetCurrentDestinationWorldSpace();
	virtual void					Unk_29();
	virtual void					Unk_2A();
	virtual void					ClearDialogTarget();
	virtual void					Unk_2C();
	virtual void					Unk_2D();
	virtual void					Unk_2E();
	virtual void					Unk_2F();
	virtual void					Unk_30();
	virtual void					Unk_31();
	virtual void					Unk_32();
	virtual void					Unk_33();
	virtual void					SetWeaponAlertDrawn();
	virtual void					Unk_35();
	virtual void					StopIdle(Actor* apkActor);
	virtual void					Unk_37();
	virtual void					Unk_38();
	virtual void					GetEssentialDownTimer();
	virtual void					SetEssentialDownTimer();
	virtual void					Unk_3B();
	virtual void					Unk_3C();
	virtual void					GetDetectingActors();
	virtual void					GetActorsGeneratedDetectionEvent();
	virtual void					CreateDetectionEvent();
	virtual void					FreeGeneratedDetectionEvent();
	virtual void					CheckForExpiredDetectionEvent();
	virtual void					Unk_42();
	virtual void					Unk_43();
	virtual bool					HasCaughtPlayerPickpocketting();
	virtual void					SetHasCaughtPlayerPickpocketting();
	virtual void					SetCurrentActionComplete();
	virtual void					Unk_47();
	virtual void					Unk_48();
	virtual void					Unk_49();
	virtual TESForm*				GetLowProcess40();
	virtual void					Unk_4B();
	virtual void					Unk_4C();
	virtual void					Unk_4D();
	virtual void					Unk_4E();
	virtual void					Unk_4F();
	virtual void					Unk_50();
	virtual BSFaceGenAnimationData*	GetFaceAnimationData();
	virtual ItemChange*				GetWeapon();
	virtual ItemChange*				GetAmmo();
	virtual void					Unk_54();
	virtual void					Unk_55();
	virtual void					Unk_56();
	virtual void					Unk_57();
	virtual void					SetWeapon(ItemChange* apkWeapon);
	virtual void					Unk_59();
	virtual void					SetAmmo(ItemChange* apkAmmo);
	virtual void					Unk_5B();
	virtual void					HandleQueuedEquipItems();
	virtual void					Unk_5D();	// Called by 5E with count itemExtraList item
	virtual void					AppendQueuedEquipItem();	// EquipItem and UnEquipItem doEquip item count itemExtraList bytes = [equipArgC lockUnequip unk unEquipArcC lockEquip arg14 ] (arg as from Actor::(Un)EquipItem)	 
	virtual void					RemoveQueuedItem();
	virtual void					RemoveQueuedEquipItems();
	virtual NiNode*					GetProjectileNode();
	virtual void					SetProjectileNode(NiNode* apkNode);
	virtual void					SetupWeaponNodeUnk_63();
	virtual NiNode*					GetWeaponBone(void* bipedNames);
	virtual void					Unk_65();
	virtual void					Unk_66();
	virtual void					Unk_67();
	virtual NiNode*					GetWeaponNode();
	virtual NiNode*					GetBowNode();
	virtual bool					IsUsingOneHandGrenade();
	virtual bool					IsUsingOneHandMine();
	virtual bool					IsUsingThrownWeapon();
	virtual bool					IsUsingOneHandThrown();
	virtual AnimData*				GetAnimData();
	virtual void					Unk_6F();
	virtual void					Unk_70();
	virtual void					Unk_71();
	virtual void					Unk_72();
	virtual void					Unk_73();
	virtual void					Unk_74();
	virtual void					Unk_75();
	virtual void					Unk_76();
	virtual void					Unk_77();
	virtual void					Unk_78();
	virtual void					Unk_79();
	virtual void					Unk_7A();
	virtual bool					Unk_7B();
	virtual void					Unk_7C();
	virtual void					Unk_7D();
	virtual void					Unk_7E();
	virtual void					Unk_7F();
	virtual void					CheckIfThereSomeoneTalkWith();
	virtual void					Unk_81();
	virtual void					Unk_82();
	virtual TESPackage*				GetRunOncePackage();
	virtual void					SetInterruptPackage(TESPackage* package, Actor* onActor);
	virtual void					StopInterruptPackage();
	virtual void					Unk_86();	// 086 - SetInterruptPackageTargetRef
	virtual void					Unk_87();	// 087 - SetInterruptPackageTargetRef
	virtual void					Unk_88();	// 088 - IncreaseInterruptPackageUnk00C
	virtual void					ClearGreetingInfoData();
	virtual void					Unk_8A();
	virtual TESPackage*				GetCurrentPackage();
	virtual void					SetCurrentPackage(TESPackage* package, Actor* onActor);
	virtual void					ClearCurrentPackage();
	virtual void					SetRunningPackageProcedureIndex();
	virtual void					GetRunningPackageProcedureIndex();
	virtual void					IncreaseRunningPackageProcedureIndex();
	virtual void					SetupNewPackage();
	virtual void					Unk_92();	// Only HighProcess, get Unk0454
	virtual void					Unk_93();
	virtual void					Unk_94();
	virtual void					GetAvoidNodes();
	virtual void					Unk_96();
	virtual void					Unk_97();
	virtual void					Unk_98();
	virtual void					GetHeadTrackTimer();
	virtual void					SetHoldHeadTrackTimer();
	virtual void					Unk_9B();
	virtual TESPackageData*			GetRunOncePackageData();
	virtual TESPackageData*			GetPackageData();
	virtual void					GetRunningActorPackage();
	virtual TESPackage*				GetCurrentPackage2();
	virtual UInt32					GetProcedureIndexRunning();
	virtual void					SetProcedureIndexRunning(UInt32);
	virtual void					IncreaseProcedureIndexRunning();
	virtual bhkCharacterController*	GetCharacterController();
	virtual void					SetCharacterController(bhkCharacterController* charCtrl);
	virtual void					StopMoving();
	virtual void					ProcessFollow();
	virtual void					ProcessPursue();
	virtual void					ProcessFlee();
	virtual void					ProcessGreet();
	virtual void					ProcessObserveCombat();
	virtual void					StopInteractingQuick();
	virtual void					Unk_AC();
	virtual void					Unk_AD();
	virtual void					Unk_AE();
	virtual void					ProcessTravel();
	virtual void					Unk_B0();
	virtual void					Unk_B1();
	virtual void					SetDetectionTimer();
	virtual void					Unk_B3();
	virtual void					Unk_B4();
	virtual void					Unk_B5();
	virtual bool					GetIsInDialogMenu();
	virtual void					SetInDialogMenu(bool abInDialog);
	virtual void					Unk_B8();
	virtual void					Unk_B9();
	virtual void					Unk_BA();
	virtual void					Unk_BB();
	virtual void					HandleRunDetection();
	virtual void					Unk_BD();
	virtual void					Unk_BE();
	virtual void					SetDiveBreath(Float32 breath);
	virtual Float32					GetDiveBreath();
	virtual void					Unk_C1();
	virtual void					Unk_C2();
	virtual bool					GetGreetingFlag();
	virtual void					SetGreetingFlag(bool);
	virtual void					Unk_C5();
	virtual void					Unk_C6();
	virtual bool					GetAlerted();
	virtual void					SetAlert(bool alert);
	virtual bool					GetByte3E0();
	virtual void					SetByte3E0(bool);
	virtual void					Unk_CB();
	virtual void					Unk_CC();
	virtual void					Unk_CD();
	virtual void					Unk_CE();
	virtual void					EnterCombat();
	virtual void					Unk_D0();
	virtual void					Unk_D1();
	virtual void					SetNumberGuardsArresting();
	virtual void					CalculateMoveMode();
	virtual void					SetActorsAnimation();
	virtual void					FinishDying();
	virtual void					Unk_D6();
	virtual void					IsRunningRunOnce();
	virtual void					IsAFollower();
	virtual void					Unk_D9();
	virtual void					GetEquippedWeaponHealthBracket();
	virtual void					Unk_DB();
	virtual void					SetByte3D0();
	virtual void					Unk_DD();
	virtual void					GetByte3D1();
	virtual void					GetLipSyncAnim();
	virtual void					SetLipSyncAnim();
	virtual void					SetHasLiveGrenade();
	virtual void					GetHasLiveGrenade();
	virtual TESIdleForm*			GetLastIdlePlayed();
	virtual void					SetForcedIdle(TESIdleForm* idleForm); // sets values of previous function
	virtual void					StopIdle2();
	virtual void					Unk_E6();
	virtual void					Unk_E7();	// Float32 GetActorValue
	virtual void					Unk_E8();
	virtual void					Unk_E9();
	virtual void					Unk_EA();
	virtual void					Unk_EB();
	virtual void					Unk_EC(UInt32 avCode);
	virtual void					Unk_ED();
	virtual void					Unk_EE();
	virtual void					Unk_EF();
	virtual void					Unk_F0();
	virtual void					Unk_F1();
	virtual void					Unk_F2();
	virtual void					Unk_F3();
	virtual void					Unk_F4();
	virtual void					Unk_F5();
	virtual void					Unk_F6();
	virtual void					Unk_F7();
	virtual void					Unk_F8();
	virtual SInt16					GetCurrentAnimAction();
	virtual BSAnimGroupSequence*	GetCurrentSequence();
	virtual void					SetCurrentActionAndSequence(int action, BSAnimGroupSequence* sequence);
	virtual void					GetForceFireWeapon();
	virtual void					SetForceFireWeapon(char);
	virtual bool					IsReadyForAnim();
	virtual void					Unk_FF();
	virtual void					SetIsAiming(bool isAiming);
	virtual bool					IsAiming();
	virtual void					Unk_102();
	virtual SInt32					GetKnockedState();
	virtual void					SetKnockedState(char state);
	virtual void					Unk_105();
	virtual void					PushActorAway(Actor* pushed, Float32 posX, Float32 posY, Float32 posZ, Float32 force);
	virtual void					UpdateKnockState();
	virtual void					Unk_108();
	virtual void					Unk_109();
	virtual void					Unk_10A();
	virtual float					GetAngle1D0();
	virtual void					Unk_10C();
	virtual void					ResetAttackLoopTimer();
	virtual void					DecreaseAttackLoopShootTime();
	virtual void					SetByte1D8();
	virtual bool					GetByte1D8();
	virtual void					Unk_111();
	virtual void					Unk_112();
	virtual bool					WantsWeaponOut();
	virtual void					SetWantsWeaponOut(bool);
	virtual bool					IsWeaponOut() const;
	virtual void					SetWeaponOut(Actor* actor, bool weaponOut);
	virtual void					Unk_117();
	virtual void					Unk_118();
	virtual void					Unk_119(Actor* actor);
	virtual void					Set3DUpdateFlag(UInt32 unk);
	virtual void					Unk_11B();
	virtual void					Unk_11C();
	virtual bool					GetFlag18C(UInt32 arg);
	virtual void					Unk_11E();
	virtual void					Unk_11F();
	virtual void					Unk_120();
	virtual void					Unk_121();
	virtual void					Unk_122();
	virtual void					GetSoundHandle();
	virtual void					SetSoundHandle();
	virtual void					StopSoundHandle();
	virtual void					Unk_126();
	virtual void					Unk_127();
	virtual void					Unk_128();
	virtual void					SetDetectionModifier();
	virtual void					SetDetectionModifierTimer();
	virtual void					Unk_12B();
	virtual void					GetDyingTimer();
	virtual void					SetDyingTimer();
	virtual void					IsTalking();
	virtual int						GetSitSleepState();
	virtual void					SetFurnitureRef();
	virtual void					GetFurnitureMarkerID();
	virtual void					GetCurrentFurnitureRef();
	virtual void					Unk_133();
	virtual void					Unk_134();
	virtual void					GetFurnitureMark();
	virtual void					Unk_136();
	virtual void					Unk_137();
	virtual void					IsContinuingPackagePCNear();
	virtual void					Unk_139();
	virtual void					Unk_13A();
	virtual void					SetTargetActivated();
	virtual void					GetAttacked();
	virtual void					Unk_13D();
	virtual void					Unk_13E();
	virtual void					SetLightingShaderProperty(UInt32 unk);
	virtual void					HandleLightUpdateTimer();
	virtual DetectionData*			GetDetectionData(Actor* target, UInt32 detecting);
	virtual void					Unk_142();
	virtual double					GetLightUpdateTimer();
	virtual void					SetLightUpdateTimer(double);
	virtual void					GetGenericLocation();
	virtual void					Unk_146();
	virtual void					Unk_147();
	virtual void					Unk_148();
	virtual void					ClearCurrentDataforProcess();
	virtual void					SetCommandingActor(Actor*);
	virtual Actor*					GetCommandingActor();
	virtual void					Unk_14C();
	virtual void					Unk_14D();
	virtual void					Unk_14E();
	virtual void					Unk_14F();
	virtual void					Unk_150();
	virtual void					Unk_151();
	virtual void					Unk_152();
	virtual void					Unk_153();
	virtual void					Unk_154();
	virtual void					LoadGame();
	virtual void					Unk_156();
	virtual void					Unk_157();
	virtual void					Unk_158();
	virtual void					Unk_159();
	virtual void					Unk_15A();
	virtual void					Unk_15B();
	virtual void					FinishLoadGame();
	virtual void					GetEnchantEffectShader();
	virtual void					SetEnchantEffectShader();
	virtual void					GetByte17C();
	virtual void					Unk_160();
	virtual void					Unk_161();
	virtual void					Unk_162();
	virtual void					Unk_163();
	virtual void					Unk_164();
	virtual void					Unk_165();
	virtual void					Unk_166();
	virtual void					Unk_167();
	virtual void					Unk_168();
	virtual void					Unk_169();
	virtual void					Unk_16A();
	virtual float					GetActorAlpha();
	virtual void					SetActorAlpha(float afAlpha);
	virtual void					Unk_16D();
	virtual void					Unk_16E();
	virtual void					Unk_16F();
	virtual void					GetNumberGuardsArresting();
	virtual void					ModNumberGuardsArresting();
	virtual void					Unk_172();
	virtual void					Unk_173();
	virtual void					Unk_174();
	virtual void					Unk_175();
	virtual void					Unk_176();
	virtual void					Unk_177();
	virtual void					Unk_178();
	virtual void					Unk_179();
	virtual void					Unk_17A();
	virtual void					Unk_17B();
	virtual BSBound*				GetBound();
	virtual void					SetBound(BSBound*);
	virtual void					SetForceNextUpdate(bool);
	virtual bool					GetForceNextUpdate();
	virtual void					SetForceSpeakToPlayerAfterGettingUp(bool);
	virtual bool					IsForceSpeakToPlayerAfterGettingUp();
	virtual void					Unk_182();
	virtual void					Unk_183();
	virtual UInt32					GetFadeType();
	virtual void					SetQueuedIdleFlags(UInt32 flags);
	virtual UInt32					GetQueuedIdleFlags();
	virtual void					ResetQueuedIdleFlags();
	virtual void					UnsetQueuedIdleFlag();
	virtual void					SetDefaultHeadTrackTarget(Actor* apActor);
	virtual void					SetActionHeadTrackTarget(Actor* apActor);
	virtual void					SetScriptHeadTrackTarget(Actor* apActor);
	virtual void					SetCombatHeadTrackTarget(Actor* apActor);
	virtual void					SetDialogHeadTrackTarget(Actor* apActor);
	virtual void					SetProcedureHeadTrackTarget(Actor* apActor);
	virtual void					SetNthHeadTrackTarget();
	virtual void					Unk_190();
	virtual void					ClearActionHeadTrackTarget();
	virtual void					ClearScriptHeadTrackTarget(UInt8 unk);
	virtual void					Unk_193();
	virtual void					Unk_194();
	virtual void					Unk_195();
	virtual void					Unk_196();
	virtual void					Unk_197();
	virtual void					Unk_198();
	virtual void					Unk_199();
	virtual void					CanSetDefaultHeadTrackTarget();
	virtual void					Unk_19B();
	virtual void					Unk_19C();
	virtual void					GetCurrentHeadTrackTarget();
	virtual void					Unk_19E();
	virtual void					Unk_19F();
	virtual void					Unk_1A0();
	virtual void					Unk_1A1();
	virtual void					Unk_1A2();
	virtual void					Unk_1A3();
	virtual void					Unk_1A4();
	virtual void					Unk_1A5();
	virtual void					Unk_1A6();
	virtual float					GetGameDayDied();
	virtual void					Unk_1A8();
	virtual void					AddArrowProjectile();
	virtual void					Unk_1AA();
	virtual void					Unk_1AB();
	virtual void					Unk_1AC();
	virtual void					SetupProjectileData();
	virtual void					GetMuzzleFlash();
	virtual void					Unk_1AF();
	virtual void					Unk_1B0();
	virtual void					Unk_1B1();
	virtual void					Unk_1B2();
	virtual void					SetNthLimbNode();
	virtual void					GetNthLimbNode();
	virtual void					Unk_1B5();
	virtual void					Unk_1B6();
	virtual bool					IsFiringAutomaticWeapon();
	virtual void					SetIsFiringAutomaticWeapon(bool);
	virtual void					UpdateIsWearingHeavyArmor();
	virtual void					IsWearingHeavyArmor();
	virtual bool					IsWearingPowerArmorTorso();
	virtual bool					IsWearingPowerArmorHelmet();
	virtual bool					IsWearingBackpack();
	virtual void					Unk_1BE();
	virtual void					Unk_1BF();
	virtual void					Unk_1C0();
	virtual void					Unk_1C1();
	virtual void					Unk_1C2();
	virtual void					PlayCrippledLimbAnim();
	virtual void					Unk_1C4();
	virtual void					Unk_1C5();
	virtual TESIdleForm*			GetQueuedIdleForm();
	virtual void					SetQueuedIdleForm(TESIdleForm* apkIdleForm);
	virtual void					Unk_1C8();
	virtual void					Unk_1C9();
	virtual void					Unk_1CA();
	virtual void					AddPlayerDamageDealt();
	virtual void					GetPlayerHealthDamageDealt();
	virtual Float32					GetLightAmount();
	virtual void					SetLightAmount(Float32 afLightAmount);
	virtual void					AddDefferedHideLimb();
	virtual void					Unk_1D0();
	virtual void					SetLipSyncAnim1A0();
	virtual void					GetLipSyncAnim1A0();
	virtual void					Unk_1D3();
	virtual void					Unk_1D4();
	virtual void					Unk_1D5();
	virtual void					DecreaseRads238();
	virtual void					GetRads238();
	virtual void					Unk_1D8();
	virtual void					GetWaterRadsSec();
	virtual void					Unk_1DA();
	virtual Float32					GetRadsSec();
	virtual ActorHitData*			GetLastHitData();
	virtual void					CopyLastHitData(ActorHitData* apkHitData);
	virtual void					ResetLastHitData();
	virtual ActorHitData*			GetLastTargetHitData();
	virtual void					CopyLastTargetHitData(ActorHitData* apkHitData);
	virtual void					ResetLastTargetHitData();
	virtual void					Unk_1E2();
	virtual void					Unk_1E3();
	virtual BSFaceGenNiNode*		GetFaceGenNiNode();
	virtual void					SetUnk0248();
	virtual BSFaceGenNiNode*		GetFaceGenNiNode2();
	virtual void					Unk_1E7();
	virtual NiTriShape*				GetTriShape();
	virtual void					Unk_1E9();
	virtual void					Unk_1EA();
	virtual void					Unk_1EB();
	virtual void					Unk_1EC();

	ActorPackage		kCurrentPackage;		// 04
	Float32				unk1C_maybeUnused;		// 1C
	Float32				fHourLastProcessed;		// 20
	UInt32				uiDateLastProcessed;	// 24	not initialized, only by descendant!
	EnumProcessLevel	eProcessLevel;			// 28	not initialized, only by descendant to 3 for Low, 2 for MiddleLow, 1 MiddleHighProcess and 0 for HighProcess
	CachedValues*		pkCachedValues;			// 2C

	// 0x8ACED0
	__forceinline bool GetCachedFlag(UInt32 auiFlag) const {
		return pkCachedValues && (pkCachedValues->eFlags & auiFlag);
	}

	__forceinline void SetCachedFlag(UInt32 auiFlag, bool abSet) {
		if (pkCachedValues)
			pkCachedValues->eFlags = (CachedValues::EnumFlags) (abSet ? pkCachedValues->eFlags | auiFlag : pkCachedValues->eFlags & ~auiFlag);
	}
};
static_assert(sizeof(BaseProcess) == 0x30);
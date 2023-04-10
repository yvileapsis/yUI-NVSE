#pragma once
#include <NiNodes.h>
#include <Types.h>
#include <BSExtraData.h>

enum AnimAction : SInt16
{
	kAnimAction_None = -0x1,
	kAnimAction_Equip_Weapon = 0x0,
	kAnimAction_Unequip_Weapon = 0x1,
	kAnimAction_Attack = 0x2,
	kAnimAction_Attack_Eject = 0x3,
	kAnimAction_Attack_Follow_Through = 0x4,
	kAnimAction_Attack_Throw = 0x5,
	kAnimAction_Attack_Throw_Attach = 0x6,
	kAnimAction_Block = 0x7,
	kAnimAction_Recoil = 0x8,
	kAnimAction_Reload = 0x9,
	kAnimAction_Stagger = 0xA,
	kAnimAction_Dodge = 0xB,
	kAnimAction_Wait_For_Lower_Body_Anim = 0xC,
	kAnimAction_Wait_For_Special_Idle = 0xD,
	kAnimAction_Force_Script_Anim = 0xE,
	kAnimAction_ReloadLoopStart = 0xF,
	kAnimAction_ReloadLoopEnd = 0x10,
	kAnimAction_ReloadLoop = 0x11,
};

enum QueuedIdleFlags
{
	kIdleFlag_FireWeapon						= 0x1,
	kIdleFlag_Reload							= 0x2,
	kIdleFlag_CrippledLimb						= 0x10,
	kIdleFlag_Death								= 0x20,
	kIdleFlag_ForcedIdle						= 0x80,
	kIdleFlag_HandGrip							= 0x100,
	kIdleFlag_Activate							= 0x400,
	kIdleFlag_StandingLayingDownChange			= 0x800,
	kIdleFlag_EquipOrUnequip					= 0x4000,
	kIdleFlag_AimWeapon							= 0x10000,
	kIdleFlag_AttackEjectEaseInFollowThrough	= 0x20000,
	kIdleFlag_SomethingAnimatingReloadLoop		= 0x40000,
};

// Straight from OBSE. Needs to be debugged ! ! ! 
// This is used all over the game code to manage actors and occassionally other objects.
class ActorProcessLists
{
public:
	ActorProcessLists();
	~ActorProcessLists();

	struct ActorList {
		TList<Actor>			head;
		TList<Actor>::Node*		tail;
	};

	ActorList				middleHighActors;		// 00
	ActorList				lowActors0C;			// 0C significantly smaller list than lowActors18. 
													//	  Plausible: actors currently scheduled for low processing
	ActorList				lowActors18;			// 18
	float					unk24;					// 24
	UInt32					unk28;					// 28
	void*					unk2C;					// 2C
	UInt32					unk30;					// 30
	void*					unk34;					// 34
	UInt32					unk38[0x0A];			// 38
	// almost confirmed to be at index 60
	TList<BSTempEffect>		tempEffects;			// 60
	// needs recalc of offsets
	UInt32					unk4C[4];				// 4C
	TList<Actor>			highActors;				// 5C
	Actor*					actor64;				// 64
	TList<Actor>::Node*		unkNodes[3];			// 68 ##TODO: which lists do these belong to
	UInt32					unk74;					// 74 Possibly not a member. Definitely no more members following this.
};

extern ActorProcessLists * g_actorProcessLists;

class AmmoInfo : public InventoryChanges
{
public:
	UInt32			unk0C;	// 0C
	UInt32			unk10;	// 10
	UInt32			unk14;	// 14
	UInt32			unk18;	// 18
	UInt32			unk1C;	// 1C
	UInt32			unk20;	// 20
	UInt32			unk24;	// 24
	UInt32			unk28;	// 28
	UInt32			unk2C;	// 2C
	UInt32			unk30;	// 30
	UInt32			unk34;	// 34
	UInt32			unk38;	// 38
	UInt32			unk3C;	// 3C
	UInt32			unk40;	// 40
	TESObjectWEAP*	ammoWeapon;	// 44
};

struct PackageInfo
{
	TESPackage*			package;		// 00
	union								// 04
	{
		TESPackageData* packageData;
		void*			actorPkgData;
	};
	TESObjectREFR*		targetRef;		// 08
	UInt32				unk0C;			// 0C	Initialized to 0FFFFFFFFh, set to 0 on start
	float				unk10;			// 10	Initialized to -1.0	. Set to GameHour on start so some time
	UInt32				flags;			// 14	Flags, bit0 would be not created and initialized
};

class WeaponInfo : public InventoryChanges {};

class BaseProcess
{
public:


	BaseProcess() {};
	virtual ~BaseProcess() {};
	virtual void			Copy() {};
	virtual void			CopyPackage() {};
	virtual void			Unk_03() {};
	virtual void			Update() {};
	virtual void			Unk_05() {};
	virtual void			Unk_06() {};
	virtual void			Unk_07() {};
	virtual void			Unk_08() {};
	virtual void			Unk_09() {};
	virtual void			Unk_0A() {};
	virtual void			Unk_0B() {};
	virtual void			Unk_0C() {};
	virtual void			Unk_0D() {};
	virtual void			Unk_0E() {};
	virtual void			Unk_0F() {};
	virtual void			GetUnk0B4() {};
	virtual void			Unk_11() {};
	virtual void			Unk_12() {};
	virtual void			Unk_13() {};
	virtual void			Unk_14() {};
	virtual void			Unk_15() {};
	virtual void			Unk_16() {};
	virtual void			Unk_17() {};
	virtual void			GetIdleDoneOnce() {};
	virtual void			Unk_19() {};
	virtual void			Unk_1A() {};
	virtual void			Unk_1B() {};
	virtual void			Unk_1C() {};
	virtual void			Unk_1D() {};
	virtual void			Unk_1E() {};
	virtual void			Unk_1F() {};
	virtual void			Unk_20() {};
	virtual void			Unk_21() {};
	virtual void			Unk_22() {};
	virtual void			Unk_23() {};
	virtual void			Unk_24() {};
	virtual void			Unk_25() {};
	virtual void			Unk_26() {};
	virtual void			Unk_27() {};
	virtual void			Unk_28() {};
	virtual void			Unk_29() {};
	virtual void			Unk_2A() {};
	virtual void			Unk_2B() {};
	virtual void			Unk_2C() {};
	virtual void			Unk_2D() {};
	virtual void			Unk_2E() {};
	virtual void			Unk_2F() {};
	virtual void			Unk_30() {};
	virtual void			Unk_31() {};
	virtual void			Unk_32() {};
	virtual void			Unk_33() {};
	virtual void			Unk_34() {};
	virtual void			Unk_35() {};
	virtual void			StopIdle(Actor*) {};
	virtual void			Unk_37() {};
	virtual void			Unk_38() {};
	virtual void			Unk_39() {};
	virtual void			SetUnkA4() {};
	virtual void			Unk_3B() {};
	virtual void			Unk_3C() {};
	virtual void			Unk_3D() {};
	virtual void			Unk_3E() {};
	virtual void			Unk_3F() {};
	virtual void			Unk_40() {};
	virtual void			CheckForExpiredDetectionEvent() {};
	virtual void			Unk_42() {};
	virtual void			Unk_43() {};
	virtual bool			GetCaughtPlayerPickpocketting() {};
	virtual void			SetCaughtPlayerPickpocketting() {};
	virtual void			Unk_46() {};
	virtual void			Unk_47() {};
	virtual void			Unk_48() {};
	virtual void			Unk_49() {};
	virtual void			GetUnk040() {};		// Due to context, could be GetCombatTarget
	virtual void			Unk_4B() {};
	virtual void			Unk_4C() {};
	virtual void			Unk_4D() {};
	virtual void			Unk_4E() {};
	virtual void			Unk_4F() {};
	virtual void			Unk_50() {};
	virtual WeaponInfo*		GetWeaponInfo() {};	// unk0114
	virtual AmmoInfo*		GetAmmoInfo() {};		// unk0118
	virtual void			Unk_51() {};
	virtual void			Unk_54() {};
	virtual void			Unk_55();
	virtual void			Unk_56();
	virtual void			Unk_57();
	virtual void			ChangeWeapon();
	virtual void			Unk_59();
	virtual void			UpdateAmmoInfo(AmmoInfo* ammoInfo);
	virtual void			Unk_5B();
	virtual void			Unk_5C();
	virtual void			Unk_5D();	// Called by 5E with count itemExtraList item
	virtual void			Unk_5E();	// EquipItem and UnEquipItem doEquip item count itemExtraList bytes = [equipArgC lockUnequip unk unEquipArcC lockEquip arg14 ] (arg as from Actor::(Un)EquipItem)
	virtual void			Unk_5F();
	virtual void			Unk_60();
	virtual NiNode*			GetProjectileNode();
	virtual void			SetProjectileNode(NiNode* node);
	virtual void			Unk_63();

	virtual NiNode*			GetWeaponBone(void* bipedNames);
	virtual void			Unk_65();
	virtual void			Unk_66();
	virtual void			Unk_67();
	virtual void			Unk_68();
	virtual void			Unk_69();
	virtual void			Unk_6A();
	virtual void			Unk_6B();
	virtual void			Unk_6C();
	virtual void			Unk_6D();
	virtual AnimData*		GetAnimData();
	virtual void			Unk_6F();
	virtual void			Unk_70();
	virtual void			Unk_71();
	virtual void			Unk_72();
	virtual void			Unk_73();
	virtual void			Unk_74();
	virtual void			Unk_75();
	virtual void			Unk_76();
	virtual void			Unk_77();
	virtual void			Unk_78();
	virtual void			Unk_79();
	virtual void			Unk_7A();
	virtual void			Unk_7B();
	virtual void			Unk_7C();
	virtual void			Unk_7D();
	virtual void			Unk_7E();
	virtual void			Unk_7F();
	virtual void			Unk_80();
	virtual void			Unk_81();
	virtual void			Unk_82();
	virtual void			GetInterruptPackage();	// 083 - GetInterruptPackage
	virtual void			SetInterruptPackage();	// 084 - SetInterruptPackage
	virtual void			StopInterruptPackage();	// 085 - StopInterruptPackage
	virtual void			Unk_86();	// 086 - SetInterruptPackageTargetRef
	virtual void			Unk_87();	// 087 - SetInterruptPackageTargetRef
	virtual void			Unk_88();	// 088 - IncreaseInterruptPackageUnk00C
	virtual void			Unk_89();
	virtual void			Unk_8A();
	virtual void			GetStablePackage();	// 08B - GetStablePackage
	virtual void			SetStablePackage();	// 08C - SetStablePackage
	virtual void			StopStablePackage();	// 08D - StopStablePackage
	virtual void			SetStablePackageUnk00C();
	virtual void			GetStablePackageUnk00C();
	virtual void			IncreaseStablePackageUnk00C();
	virtual void			Unk_91();
	virtual void			Unk_92();	// Only HighProcess, get Unk0454
	virtual void			Unk_93();
	virtual void			Unk_94();
	virtual void			Unk_95();
	virtual void			Unk_96();
	virtual void			Unk_97();
	virtual void			Unk_98();
	virtual void			Unk_99();
	virtual void			Unk_9A();
	virtual void			Unk_9B();
	virtual void			Unk_9C();
	virtual TESPackageData*	GetPackageData();
	virtual void			Unk_9E();
	virtual TESPackage*		GetCurrentPackage();
	virtual UInt32			GetPackageInfo0C();
	virtual void			Unk_A1();
	virtual void			Unk_A2();
	virtual bhkCharacterController* GetCharacterController();
	virtual void			SetCharacterController(bhkCharacterController* charCtrl);
	virtual void			Unk_A5();
	virtual void			Unk_A6();
	virtual void			Unk_A7();
	virtual void			Unk_A8();
	virtual void			Unk_A9();
	virtual void			Unk_AA();
	virtual void			Unk_AB();
	virtual void			Unk_AC();
	virtual void			Unk_AD();
	virtual void			Unk_AE();
	virtual void			Unk_AF();
	virtual void			Unk_B0();
	virtual void			Unk_B1();
	virtual void			Unk_B2();
	virtual void			Unk_B3();
	virtual void			Unk_B4();
	virtual void			Unk_B5();
	virtual void			Unk_B6();
	virtual void			Unk_B7();
	virtual void			Unk_B8();
	virtual void			Unk_B9();
	virtual void			Unk_BA();
	virtual void			Unk_BB();
	virtual void			Unk_BC();
	virtual void			Unk_BD();
	virtual void			Unk_BE();
	virtual void			SetDiveBreath(float breath);
	virtual float			GetDiveBreath();
	virtual void			Unk_C1();
	virtual void			Unk_C2();
	virtual void			Unk_C3();
	virtual void			Unk_C4();
	virtual void			Unk_C5();
	virtual void			Unk_C6();
	virtual bool			GetAlerted();
	virtual void			SetAlert(bool alert);
	virtual void			Unk_C9();
	virtual void			Unk_CA();
	virtual void			Unk_CB();
	virtual void			Unk_CC();
	virtual void			Unk_CD();
	virtual void			Unk_CE();
	virtual void			Unk_CF();
	virtual void			Unk_D0();
	virtual void			Unk_D1();
	virtual void			Unk_D2();
	virtual void			Unk_D3();
	virtual void			Unk_D4();
	virtual void			Unk_D5();
	virtual void			Unk_D6();
	virtual void			Unk_D7();
	virtual void			Unk_D8();
	virtual void			Unk_D9();
	virtual void			Unk_DA();
	virtual void			Unk_DB();
	virtual void			Unk_DC();
	virtual void			Unk_DD();
	virtual void			Unk_DE();
	virtual void			Unk_DF();
	virtual void			Unk_E0();
	virtual void			Unk_E1();
	virtual void			Unk_E2();
	virtual TESIdleForm*	GetIdleForm10C();
	virtual void			SetIdleForm10C(TESIdleForm* idleForm);
	virtual void			StopIdle();
	virtual void			Unk_E6();
	virtual void			Unk_E7();	// float GetActorValue
	virtual void			Unk_E8();
	virtual void			Unk_E9();
	virtual void			Unk_EA();
	virtual void			Unk_EB();
	virtual void			Unk_EC();
	virtual void			Unk_ED();
	virtual void			Unk_EE();
	virtual void			Unk_EF();
	virtual void			Unk_F0();
	virtual void			Unk_F1();
	virtual void			Unk_F2();
	virtual void			Unk_F3();
	virtual void			Unk_F4();
	virtual void			Unk_F5();
	virtual void			Unk_F6();
	virtual void			Unk_F7();
	virtual void			Unk_F8();
	virtual SInt16			GetCurrentAnimAction();
	virtual BSAnimGroupSequence* GetCurrentSequence();
	virtual void			SetCurrentActionAndSequence(int action, BSAnimGroupSequence* sequence);
	virtual void			GetForceFireWeapon();
	virtual void			SetForceFireWeapon();
	virtual bool			IsReadyForAnim();
	virtual void			Unk_FF();
	virtual void			SetIsAiming();
	virtual bool			IsAiming();
	virtual void			Unk_102();
	virtual void			GetKnockedState();
	virtual void			SetKnockedState();
	virtual void			Unk_105();
	virtual void			PushActorAway();
	virtual void			Unk_107();
	virtual void			Unk_108();
	virtual void			Unk_109();
	virtual void			Unk_10A();
	virtual void			Unk_10B();
	virtual void			Unk_10C();
	virtual void			ResetAttackLoopTimer(bool setToFull);
	virtual void			DecreaseAttackLoopShootTime(Actor*);
	virtual void			Unk_10F();
	virtual void			Unk_110();
	virtual void			Unk_111();
	virtual void			Unk_112();
	virtual void			Unk_113();
	virtual void			Unk_114();
	virtual bool			IsWeaponOut();
	virtual void			SetWeaponOut(Actor* actor, bool weaponOut);
	virtual void			Unk_117();
	virtual void			Unk_118();
	virtual void			Unk_119();
	virtual void			Unk_11A();
	virtual void			Unk_11B();
	virtual void			Unk_11C();
	virtual void			Unk_11D();
	virtual void			Unk_11E();
	virtual void			Unk_11F();
	virtual void			Unk_120();
	virtual void			Unk_121();
	virtual void			Unk_122();
	virtual void			Unk_123();
	virtual void			Unk_124();
	virtual void			Unk_125();
	virtual void			Unk_126();
	virtual void			Unk_127();
	virtual void			Unk_128();
	virtual void			Unk_129();
	virtual void			Unk_12A();
	virtual void			Unk_12B();
	virtual void			Unk_12C();
	virtual void			Unk_12D();
	virtual void			Unk_12E();
	virtual int				GetSitSleepState();
	virtual void			Unk_130();
	virtual void			Unk_131();
	virtual void			Unk_132();
	virtual void			Unk_133();
	virtual void			Unk_134();
	virtual void			Unk_135();
	virtual void			Unk_136();
	virtual void			Unk_137();
	virtual void			Unk_138();
	virtual void			Unk_139();
	virtual void			Unk_13A();
	virtual void			Unk_13B();
	virtual void			Unk_13C();
	virtual void			Unk_13D();
	virtual void			Unk_13E();
	virtual void			Unk_13F();
	virtual void			Unk_140();
	virtual DetectionData*	GetDetectionData(Actor* target, UInt32 detecting);
	virtual void			Unk_142();
	virtual void			Unk_143();
	virtual void			Unk_144();
	virtual void			Unk_145();
	virtual void			Unk_146();
	virtual void			Unk_147();
	virtual void			Unk_148();
	virtual void			Unk_149();
	virtual void			Unk_14A();
	virtual void			Unk_14B();
	virtual void			Unk_14C();
	virtual void			Unk_14D();
	virtual void			Unk_14E();
	virtual void			Unk_14F();
	virtual void			Unk_150();
	virtual void			Unk_151();
	virtual void			Unk_152();
	virtual void			Unk_153();
	virtual void			Unk_154();
	virtual void			LoadGame();
	virtual void			Unk_156();
	virtual void			Unk_157();
	virtual void			Unk_158();
	virtual void			Unk_159();
	virtual void			Unk_15A();
	virtual void			Unk_15B();
	virtual void			Unk_15C();
	virtual void			Unk_15D();
	virtual void			Unk_15E();
	virtual void			Unk_15F();
	virtual void			Unk_160();
	virtual void			Unk_161();
	virtual void			Unk_162();
	virtual void			Unk_163();
	virtual void			Unk_164();
	virtual void			Unk_165();
	virtual void			Unk_166();
	virtual void			Unk_167();
	virtual void			Unk_168();
	virtual void			Unk_169();
	virtual void			Unk_16A();
	virtual float			GetActorAlpha();
	virtual void			SetActorAlpha(float alpha);
	virtual void			Unk_16D();
	virtual void			Unk_16E();
	virtual void			Unk_16F();
	virtual void			Unk_170();
	virtual void			Unk_171();
	virtual void			Unk_172();
	virtual void			Unk_173();
	virtual void			Unk_174();
	virtual void			Unk_175();
	virtual void			Unk_176();
	virtual void			Unk_177();
	virtual void			Unk_178();
	virtual void			Unk_179();
	virtual void			Unk_17A();
	virtual void			Unk_17B();
	virtual BSBound*		GetBoundingBox();
	virtual void			Unk_17D();
	virtual void			Unk_17E();
	virtual bool			IsTeammate();
	virtual void			Unk_180();
	virtual void			Unk_181();
	virtual void			Unk_182();
	virtual void			Unk_183();
	virtual void			GetFadeType();
	virtual void			SetQueuedIdleFlag(UInt32 flag);
	virtual UInt32			GetQueuedIdleFlags();
	virtual void			ResetQueuedIdleFlags();
	virtual void			UnsetQueuedIdleFlag();
	virtual void			Unk_189();
	virtual void			Unk_18A();
	virtual void			Unk_18B();
	virtual void			Unk_18C();
	virtual void			Unk_18D();
	virtual void			Unk_18E();
	virtual void			Unk_18F();
	virtual void			Unk_190();
	virtual void			Unk_191();
	virtual void			Unk_192();
	virtual void			Unk_193();
	virtual void			Unk_194();
	virtual void			Unk_195();
	virtual void			Unk_196();
	virtual void			Unk_197();
	virtual void			Unk_198();
	virtual void			Unk_199();
	virtual void			Unk_19A();
	virtual void			Unk_19B();
	virtual void			Unk_19C();
	virtual void			Unk_19D();
	virtual void			Unk_19E();
	virtual void			Unk_19F();
	virtual void			Unk_1A0();
	virtual void			Unk_1A1();
	virtual void			Unk_1A2();
	virtual void			Unk_1A3();
	virtual void			Unk_1A4();
	virtual void			Unk_1A5();
	virtual void			Unk_1A6();
	virtual void			Unk_1A7();
	virtual void			Unk_1A8();
	virtual void			Unk_1A9();
	virtual void			Unk_1AA();
	virtual void			Unk_1AB();
	virtual void			Unk_1AC();
	virtual void			Unk_1AD();
	virtual void			Unk_1AE();
	virtual void			Unk_1AF();
	virtual void			Unk_1B0();
	virtual void			Unk_1B1();
	virtual void			Unk_1B2();
	virtual void			Unk_1B3();
	virtual void			Unk_1B4();
	virtual void			Unk_1B5();
	virtual void			Unk_1B6();
	virtual void			Unk_1B7();
	virtual void			Unk_1B8();
	virtual void			Unk_1B9();
	virtual void			Unk_1BA();
	virtual void			Unk_1BB();
	virtual void			Unk_1BC();
	virtual void			Unk_1BD();
	virtual void			Unk_1BE();
	virtual void			Unk_1BF();
	virtual void			Unk_1C0();
	virtual void			Unk_1C1();
	virtual void			Unk_1C2();
	virtual void			Unk_1C3();
	virtual void			Unk_1C4();
	virtual void			Unk_1C5();
	virtual TESIdleForm*	GetQueuedIdleForm();
	virtual void			SetQueuedIdleForm(TESIdleForm*);
	virtual void			Unk_1C8();
	virtual void			Unk_1C9();
	virtual void			Unk_1CA();
	virtual void			Unk_1CB();
	virtual void			Unk_1CC();
	virtual float			GetLightAmount();
	virtual void			SetLightAmount(float lightAmount);
	virtual void			Unk_1CF();
	virtual void			Unk_1D0();
	virtual void			Unk_1D1();
	virtual void			Unk_1D2();
	virtual void			Unk_1D3();
	virtual void			Unk_1D4();
	virtual void			Unk_1D5();
	virtual void			Unk_1D6();
	virtual void			Unk_1D7();
	virtual void			Unk_1D8();
	virtual void			GetWaterRadsSec();
	virtual void			Unk_1DA();
	virtual float			GetRadsSec();
	virtual ActorHitData*	GetLastHitData();
	virtual void			CopyLastHitData(ActorHitData* hitData);
	virtual void			ResetLastHitData();
	virtual ActorHitData*	GetLastTargetHitData();
	virtual void			CopyLastTargetHitData(ActorHitData* hitData);
	virtual void			ResetLastTargetHitData();
	virtual void			Unk_1E2();
	virtual void			Unk_1E3();
	virtual void			Unk_1E4();
	virtual void			Unk_1E5();
	virtual void			Unk_1E6();
	virtual void			Unk_1E7();
	virtual void			Unk_1E8();
	virtual void			Unk_1E9();
	virtual void			Unk_1EA();
	virtual void			Unk_1EB();
	virtual void			Unk_1EC();

	struct Data004 {
		TESPackage* package;		// 000
		TESPackageData* packageData;	// 004
		TESObjectREFR* targetRef;	// 008
		UInt32			unk00C;			// 00C	Initialized to 0FFFFFFFFh, set to 0 on start
		float			flt010;			// 010	Initialized to -1.0	. Set to GameHour on start so some time
		UInt32			flags;			// 014	Flags, bit0 would be not created and initialized
	};	// 018

	struct	Data02C {
		float	flt000;
		float	flt004;
		float	flt008;
		float	flt00C;
		float	flt010;
		float	flt014;
		float	flt018;
		float	flt01C;
		float	flt020;
		float	flt024;
		float	flt028;
		float	flt02C;
		UInt32	unk030;
		UInt32	unk034;
		float	flt038;
		float	flt03C;
		UInt8	byt040;
		UInt8	fil041[3];
		UInt32	unk044;			// 044	flags, bit28 = IsGhostn
	};

	PackageInfo		currentPackage;	// 04	current package ?
	float			flt01C;			// 1C	not initialized, only by descendant!
	float			flt020;			// 20	not initialized, only by descendant to -1.0! flt020 gets set to GameHour minus one on package evaluation
	UInt32			unk024;			// 24	not initialized, only by descendant!
	UInt32			processLevel;	// 28	not initialized, only by descendant to 3 for Low, 2 for MidlleLow, 1 MiddleHighProcess and 0 for HigProcess
	Data02C*		unk02C;			// 2C
	bool IsHighProcess() { return processLevel == 0; };
};
static_assert(sizeof(BaseProcess) == 0x30);

class LowProcess : public BaseProcess
{
public:

	struct FloatPair {
		float	flt000;
		float	flt004;
	};

	struct	ActorValueModifier
	{
		UInt8	actorValue;	// 00 Might allow for other values
		UInt8	pad[3];		// 01
		float	damage;		// 04
	};

	struct ActorValueModifiers
	{
		TList<ActorValueModifier>	avModifierList;	// 00
		UInt8						unk008;			// 08
		UInt8						pad009[3];		// 09
		void**						modifiedAV;	// 0C	array of damaged actorValue
	};	// 10

	LowProcess();
	~LowProcess() override;
	virtual void	Unk_1ED();
	virtual void	Unk_1EE();
	virtual void	Unk_1EF();
	virtual void	Unk_1F0();
	virtual void	Unk_1F1();
	virtual void	Unk_1F2();
	virtual void	Unk_1F3();
	virtual void	Unk_1F4();
	virtual void	Unk_1F5();
	virtual void	Unk_1F6();
	virtual void	Unk_1F7();
	virtual void	Unk_1F8();
	virtual void	Unk_1F9();
	virtual void	Unk_1FA();
	virtual void	Unk_1FB();
	virtual void	Unk_1FC();
	virtual void	Unk_1FD();
	virtual void	Unk_1FE();
	virtual void	Unk_1FF();
	virtual void	Unk_200();
	virtual void	Unk_201();
	virtual void	Unk_202();
	virtual void	Unk_203();
	virtual void	Unk_204();
	virtual void	Unk_205();
	virtual void	Unk_206();

	UInt8					byt030;		// Flags, used before being initialized . Ok, the initial value is zeroed out by a "and 0" but ???
	UInt8					pad031[3];
	UInt32					unk034;
	FloatPair				unk038;
	TESForm*				unk040;		// not initialized!	refr, expected actor, might be CombatTarget
	UInt32					unk044;
	UInt32					unk048;
	UInt32					unk04C;
	UInt32					unk050;		// not initialized!
	UInt32					unk054;
	UInt32					unk058;
	TList<UInt32>			unk05C;		// List
	UInt32					unk064;
	UInt32					unk068;
	TList<TESForm>			unk06C;
	TList<UInt32>			unk074;
	TList<UInt32>			unk07C;
	UInt32					unk084;
	UInt32					unk088;
	UInt32					unk08C;
	UInt32					unk090;
	ActorValueModifiers		damageModifiers;
	float					fltA4;				// A4
	float					gameDayDied;		// A8
	float					fltAC;				// AC
	UInt32					unkB0;
};
static_assert(sizeof(LowProcess) == 0xB4);

// LowProcess has 207 virtual func

class MiddleLowProcess : public LowProcess
{
public:
	MiddleLowProcess();
	~MiddleLowProcess() override;

	virtual void	SetAnimation(UInt32 newAnimation);

	UInt32				unk0B4;			// 0B4
	ActorValueModifiers	tempModifiers;	// 0B8
};	// 0C8
static_assert(sizeof(MiddleLowProcess) == 0x0C8);

// MiddleLowProcess has 208 virtual func, 208 would be SetAnimation


// 25C
class MiddleHighProcess : public MiddleLowProcess
{
public:
	MiddleHighProcess();
	~MiddleHighProcess() override;

	virtual void	Unk_208();
	virtual void	Unk_209();
	virtual void	Unk_20A();
	virtual void	Unk_20B();
	virtual void	Unk_20C();
	virtual void	Unk_20D();
	virtual void	Unk_20E();
	virtual void	Unk_20F();
	virtual void	Unk_210();
	virtual void	Unk_211();
	virtual void	Unk_212();
	virtual void	Unk_213();
	virtual void	Unk_214();
	virtual void	Unk_215();
	virtual void	Unk_216();
	virtual void	Unk_217();
	virtual void	Unk_218();
	virtual void	Unk_219();
	virtual void	Unk_21A();
	virtual void	Unk_21B();

	TList<TESForm*>						unk0C8;				// 0C8
	TList<UInt32>						unk0D0;				// 0D0
	UInt32								unk0D8[3];			// 0D8
	PackageInfo							interruptPackage;	// 0E4 InterruptPackage
	UInt8								unk0FC[12];			// 0FC	Saved as one, might be Pos/Rot given size
	UInt32								unk108;				// 108
	TESIdleForm*						idleForm;			// 10C
	UInt32								unk110;				// 110  EntryData, also handled as part of weapon code. AmmoInfo.
	WeaponInfo*							weaponInfo;			// 114
	AmmoInfo*							ammoInfo;			// 118
	QueuedFile*							unk11C;				// 11C
	NiRefObject*						niRefObject;		// 120
	UInt8								isUsingOneHandGrenade;
	UInt8								isUsingOneHandMine;
	UInt8								isUsingOneHandThrown;
	UInt8								isWearingHeavyArmor;
	UInt8								isWearingPowerArmorTorso;
	UInt8								isWearingPowerArmorHelmet;
	UInt8								isWearingBackpack;	// 12A
	UInt8								gap12B;				// 12B
	NiNode*								node12C;			// 12C
	NiNode*								projectileNode;		// 130  Gets copied over during TESNPC.CopyFromBase
	UInt8								byt134;				// 134
	bool								isWeaponOut;		// 135
	UInt8								byt136;				// 136
	UInt8								byt137;				// 137
	bhkCharacterController*				charCtrl;			// 138 its an animation/bhkCharacterController. Current Animation?
	UInt8								knockedState;		// 13C
	UInt8								unk13D[3];			// 13D
	UInt32								unk140[8];			// 140
	MagicItem*							magicItem160;		// 160 get/set.
	UInt32								unk164[3];			// 164
	float								actorAlpha;			// 170
	UInt32								unk174;				// 174
	BSFaceGenAnimationData*				unk178;				// 178
	UInt8								byte17C;			// 17C
	UInt8								byte17D;			// 17D
	UInt8								byte17E;			// 17E
	UInt8								byte17F;			// 17F
	UInt8								byte180;			// 180
	UInt8								byte181;			// 181
	UInt8								byte182;			// 182
	UInt8								byte183;			// 183
	UInt32								unk184;				// 184
	UInt8								hasCaughtPCPickpocketting;	// 188
	UInt8								byte189;			// 189
	UInt8								byte18A;			// 18A
	UInt8								byte18B;			// 18B
	UInt8								byte18C;			// 18C
	UInt8								forceNextUpdate;	// 18D
	UInt8								pad18E[2];			// 18E
	UInt32								unk190[10];			// 190
	void*								unk1B8;				// 1B8
	MagicTarget*						magicTarget1BC;		// 1BC MagicTarget*
	AnimData*							animData;			// 1C0
	BSAnimGroupSequence*				animSequence[3];	// 1C4 	Cleared at the same time as the animation
	Float32								angle1D0;			// 1D0
	Float32								unk1D4;				// 1D4
	UInt8								byte1D8;			// 1D8
	UInt8								usingAutoWeapon;	// 1D9
	UInt8								isAttacked;			// 1DA
	UInt8								byte1DB;			// 1DB
	NiNode*								limbNodes[15];		// 1DC
	NiNode*								unk218;				// 218
	NiNode*								unk21C;				// 21C
	RefNiObject							unk220;				// 220
	BSBound*							boundingBox;		// 224
	UInt8								isAiming;			// 228
	UInt8								byte229;			// 229
	UInt16								byte22A;			// 22A
	UInt32								unk22C;				// 22C
	TList<InventoryChanges>				queuedEquipItems;	// 230	InventoryChanges*
	float								rads238;			// 238
	float								waterRadsSec;		// 23C
	ActorHitData*						lastHitData;			// 240 ActorHitData*
	UInt32								unk244;				// 244
	BSFaceGenNiNode*					unk248;				// 248
	BSFaceGenNiNode*					unk24C;				// 24C
	NiTriShape*							unk250;				// 250
	ActorHitData*						lastTargetHitData;			// 254 ActorHitData*
	UInt32								unk258;				// 258
};
static_assert(sizeof(MiddleHighProcess) == 0x25C);

// 46C
class HighProcess : public MiddleHighProcess
{
public:

	~HighProcess() override;
	virtual void	GetList394();

	TList<Actor>*						detectedActors;		// 25C
	TList<Actor>*						detectingActors;	// 260
	void*								ptr264;				// 264
	void*								ptr268;				// 268
	void*								ptr26C;				// 26C
	UInt32								unk270;				// 270
	TList<void>							list274;			// 274
	TList<void>							list27C;			// 27C
	TList<void>							list284;			// 284
	TList<void>							list28C;			// 28C
	float								flt294;				// 294
	float								flt298;				// 298
	UInt32								unk29C;				// 29C
	float								flt2A0;				// 2A0
	UInt32								unk2A4;				// 2A4
	float								flt2A8;				// 2A8
	UInt32								unk2AC;				// 2AC
	float								actorAlpha2;		// 2B0
	float								flt2B4;				// 2B4
	float								flt2B8;				// 2B8
	float								flt2BC;				// 2BC
	UInt16								word2C0;			// 2C0
	UInt8								byte2C2;			// 2C2
	UInt8								byte2C3;			// 2C3
	UInt8								byte2C4;			// 2C4
	UInt8								byte2C5;			// 2C5
	UInt8								byte2C6;			// 2C6
	UInt8								byte2C7;			// 2C7
	float								flt2C8;				// 2C8
	UInt32								unk2CC;				// 2CC
	float								flt2D0;				// 2D0
	float								flt2D4;				// 2D4
	float								flt2D8;				// 2D8
	UInt32								unk2DC;				// 2DC
	float								flt2E0;				// 2E0
	NiBSBoneLODController*				ptr2E4;				// 2E4
	UInt32								unk2E8;				// 2E8
	AnimAction							currentAction;		// 2EC
	UInt8								pad2EE[2];			// 2EE
	BSAnimGroupSequence*				currentSequence;	// 2F0
	UInt8								forceFireWeapon;	// 2F4
	UInt8								pad2F5[3];			// 2F5
	float								flt2F8;				// 2F8
	UInt32								unk2FC[5];			// 2FC
	float								flt310;				// 310
	UInt32								unk314[6];			// 314
	UInt8								byte32C;			// 32C
	UInt8								byte32D;			// 32D
	UInt8								byte32E;			// 32E
	UInt8								byte32F;			// 32F
	float								flt330;				// 330
	float								flt334;				// 334
	float								flt338;				// 338
	float								diveBreath;			// 33C
	UInt32								unk340;				// 340
	float								flt344;				// 344
	UInt32								unk348;				// 348
	float								flt34C;				// 34C
	TESIdleForm*						idleForm350;		// 350
	UInt32								unk354[4];			// 354
	void**								ptr364;				// 364
	UInt32								unk368[4];			// 368
	float								flt378;				// 378
	float								flt37C;				// 37C
	UInt32								unk380;				// 380
	float								flt384;				// 384
	float								flt388;				// 388
	TList<void>							list38C;			// 38C
	TList<void>							list394;			// 394
	UInt32								unk39C;				// 39C
	UInt32								unk3A0;				// 3A0
	float								flt3A4;				// 3A4
	UInt32								unk3A8[5];			// 3A8
	float								flt3BC;				// 3BC
	float								flt3C0;				// 3C0
	float								lightAmount;		// 3C4
	float								flt3C8;				// 3C8
	UInt32								unk3CC;				// 3CC
	UInt32								unk3D0;				// 3D0
	void*								projData;			// 3D4
	UInt32								unk3D8;				// 3D8
	void*								detectionEvent;		// 3DC
	UInt32								unk3E0;				// 3E0
	UInt32								unk3E4;				// 3E4
	UInt32								fadeType;			// 3E8
	float								delayTime;			// 3EC
	UInt32								unk3F0;				// 3F0
	UInt32								unk3F4;				// 3F4
	UInt32								unk3F8[3];			// 3F8
	Actor*								combatTarget;		// 404
	UInt32								unk408[4];			// 408
	float								flt418;				// 418
	TESObjectREFR*						packageTarget;		// 41C
	UInt32								unk420;				// 420
	UInt32								queuedIdleFlags;	// 424
	UInt32								unk428;				// 428
	float								flt42C;				// 42C
	UInt32								unk430;				// 430
	void*								ptr434;				// 434
	UInt32								unk438;				// 438
	float								unk43C;				// 43C
	float								radsSec440;			// 440
	UInt8								plantedExplosive;	// 444
	UInt8								pad445[3];			// 445
	float								flt448;				// 448
	UInt32								unk44C;				// 44C
	float								flt450;				// 450
	UInt32								unk454[6];			// 454
};
static_assert(sizeof(HighProcess) == 0x46C);

class AnimIdle : public NiRefObject
{
public:
	UInt32 unk008;
	UInt32 unk00C;
	UInt32 unk010;
	UInt32 sequenceID;
	BSAnimGroupSequence* unk018;
	NiRefObject* unk01C[2];
	NiRefObject* unk024[2];
	TESIdleForm* idleForm;
	UInt32 unk030;
	Actor* actor;
};
static_assert(sizeof(AnimIdle) == 0x38);

class AnimSequenceBase
{
public:
	virtual void Destroy(bool doFree);
	virtual void AddAnimGroupSequence(BSAnimGroupSequence* sequence, int unused);
	virtual bool RemoveAnimGroupSequence(BSAnimGroupSequence* sequence, int unused);
	virtual bool IsSingle();
	virtual BSAnimGroupSequence* GetSequenceByIndex(int index);
	virtual BSAnimGroupSequence* GetSequenceByAnimGroup(TESAnimGroup* group);
	virtual signed int GetIndexOfSequenceName(const char* name);
};

class AnimSequenceSingle : public AnimSequenceBase
{
public:
	BSAnimGroupSequence* anim;
};

// 08
class AnimSequenceMultiple : public AnimSequenceBase
{
public:
	DList<BSAnimGroupSequence>* anims; // 04
};

struct AnimData
{
	struct Unk124
	{
		struct Unk18
		{
			UInt32					unk00[9];
			UInt32					unk24;
		};

		UInt32						unk00[6];
		Unk18*						unk18;
	};

	struct Unk128
	{
		UInt32						unk00[11];
		TESIdleForm*				idle2C;
	};

	UInt32							unk000;				// 000
	Actor*  						actor;				// 004
	NiNode*  						nSceneRoot;		// 008
	NiNode*  						nBip01;			// 00C
	NiPoint3						pt010;				// 010
	NiPoint3						pt01C;				// 01C
	NiNode*  						nPelvis;			// 028
	NiNode*  						nBip01Copy;		// 02C
	NiNode*  						nLForearm;			// 030
	NiNode*							nHead;				// 034
	NiNode* 						nWeapon;			// 038
	NiNode*							UNUSED03C;			// 03C
	NiNode*							UNUSED040;			// 040
	NiNode*							nNeck1;			// 044
	float							unk048;				// 048
	UInt16							groupIDs[8];		// 04C
	SInt32							sequenceState1[8];	// 05C
	SInt32							sequenceState2[8];	// 07C
	UInt16							unk09C[8];			// 09C
	UInt32							unk0AC[8];			// 0AC
	UInt8							byte0CC;			// 0CC
	UInt8							byte0CD;			// 0CD
	UInt8							byte0CE;			// 0CE
	UInt8							byte0CF;			// 0CF
	float							timePassed;			// 0D0
	UInt32							unk0D4;				// 0D4
	NiControllerManager*  			controllerManager;			// 0D8
	NiTPointerMap<AnimSequenceBase>* mapAnimSequenceBase;// 0DC
	BSAnimGroupSequence*			animSequence[8];	// 0E0
	BSAnimGroupSequence*			animSeq100;		// 100
	TList<KFModel>					loadingAnims;
	float							movementSpeedMult;
	float							rateOfFire;
	float 							turboSpeedMult;
	float 							weaponReloadSpeed;
	float 							equipSpeed;
	bool 							noBlend120;
	UInt8 							byte121;
	UInt16 							unk122;
	AnimIdle* 						idleAnim;
	AnimIdle* 						idleAnimQueued;
	NiNode* 						node12C;
	NiNode* 						node130;
	TList<void> 					list134;

	TESAnimGroup::AnimGroupID GetNextAttackGroupID() const;

};
static_assert(sizeof(AnimData) == 0x13C);


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
		NavMeshInfo* navMeshInfo;
		SInt16			triangle;
	};

	UInt32	unk000;			// 00
	Connection connectTo;	// 04
};	// Alloc'd by 0x0C

struct NavMeshTriangleDoorPortal
{
	TESObjectREFR* door;	// 00
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

class ScrapHeapQueue
{
public:
	struct QueuedCmdCall
	{
		UInt32			opcode;		// 00
		void* cmdAddr;	// 04
		UInt32			thisObj;	// 08	refID
		UInt32			numArgs;	// 0C
		FunctionArg		args[4];	// 10

		QueuedCmdCall(void* _cmdAddr, UInt32 _thisObj, UInt8 _numArgs) : opcode(0x2B), cmdAddr(_cmdAddr),
			thisObj(_thisObj), numArgs(_numArgs), args{}
		{
		}
	};

	static ScrapHeapQueue* GetSingleton() { return *reinterpret_cast<ScrapHeapQueue**>(0x11DF1A8); }
	__forceinline void			AddQueuedCmdCall(QueuedCmdCall qCall) { ThisCall(0x87D160, this, &qCall); }
};

inline const UInt32* g_TlsIndexPtr = reinterpret_cast<UInt32*>(0x0126FD98);

struct TLSData
{
	// thread local storage

	UInt32			pad000[(0x260 - 0x000) >> 2];	// 000
	NiNode* lastNiNode;						// 260	248 in FOSE
	TESObjectREFR* lastNiNodeREFR;					// 264	24C in FOSE
	UInt8			consoleMode;					// 268
	UInt8			pad269[3];						// 269
	// 25C is used as do not head track the player , 
	// 2B8 is used to init QueudFile::unk0018, 
	// 28C might count the recursive calls to Activate, limited to 5.
};


static TLSData* GetTLSData()
{
	UInt32 TlsIndex = *g_TlsIndexPtr;
	TLSData* data = nullptr;

	__asm {
		mov		ecx, [TlsIndex]
		mov		edx, fs: [2Ch]	// linear address of thread local storage array
		mov		eax, [edx + ecx * 4]
		mov[data], eax
	}

	return data;
}
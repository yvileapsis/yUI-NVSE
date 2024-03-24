#pragma once

#include "MobileObject.hpp"
#include "ActorValueOwner.hpp"
#include "CachedValuesOwner.hpp"
#include "MagicCaster.hpp"
#include "MagicTarget.hpp"
#include "ActorMover.hpp"

class BGSPerk;
class bhkRagdollController;
class bhkRagdollPenetrationUtil;
class TESActorBase;
class ContinuousBeamProjectile;
class TESPackage;

typedef std::vector<TESForm*> EquippedItemsList;

class Actor : public MobileObject, public MagicCaster, public MagicTarget, public ActorValueOwner, public CachedValuesOwner {
public:
	Actor();
	virtual ~Actor();

	virtual void		Unk_C1();
	virtual void		Unk_C2();
	virtual void		Unk_C3();
	virtual void		Unk_C4();
	virtual void		Unk_C5();
	virtual void		Unk_C6();
	virtual void		Unk_C7();
	virtual void		Unk_C8();
	virtual void		Unk_C9();
	virtual void		Unk_CA();
	virtual void		Unk_CB();
	virtual void		Unk_CC();
	virtual void		Unk_CD();
	virtual void		Unk_CE();
	virtual void		Unk_CF();
	virtual void		Unk_D0();
	virtual void		Unk_D1();
	virtual void		Unk_D2(float,bool);
	virtual void		Unk_D3();
	virtual void		Unk_D4();
	virtual void		Unk_D5();
	virtual void		Unk_D6();
	virtual void		Unk_D7();
	virtual void		Unk_D8();	// IsPlayerRef
	virtual void		Unk_D9();
	virtual void		Unk_DA();
	virtual void		Unk_DB();
	virtual void		Unk_DC();
	virtual void		Unk_DD();
	virtual void		Unk_DE();
	virtual void		Unk_DF();
	virtual void		Unk_E0();
	virtual void		SetRefraction(bool, float);
	virtual void		Unk_E2();
	virtual void		Unk_E3();
	virtual void		Unk_E4();	// Creature = 0, Character = 1, PlayerCharacter = 2
	virtual void		Unk_E5();
	virtual void		Unk_E6();
	virtual void		Unk_E7();
	virtual void		Unk_E8();
	virtual void		Unk_E9();
	virtual void		Unk_EA();
	virtual void		Unk_EB();
	virtual void		Unk_EC();
	virtual void		Unk_ED();
	virtual void		Unk_EE();
	virtual void		Unk_EF();
	virtual void		Unk_F0();
	virtual void		Unk_F1();
	virtual void		Unk_F2();
	virtual void		Unk_F3();
	virtual void		Unk_F4();
	virtual void		Unk_F5();
	virtual void		Unk_F6();
	virtual void		Unk_F7();
	virtual void		Unk_F8();	// 0F8 bool AddSpell(spell)
	virtual void		Unk_F9();
	virtual void		Unk_FA();
	virtual void		Unk_FB();
	virtual void		Unk_FC();
	virtual void		Unk_FD();
	virtual void		Unk_FE();
	virtual void		Unk_FF();
	virtual void		Unk_100();
	virtual void		Unk_101();
	virtual void		Unk_102();
	virtual void		Unk_103();
	virtual void		Unk_104();
	virtual void		Unk_105();
	virtual void		Unk_106();
	virtual float		GetAlpha();
	virtual void		Unk_108();
	virtual void		Unk_109();
	virtual bool		IsInCombat();
	virtual Actor*		GetCombatTarget();
	virtual void		Unk_10C();
	virtual void		Unk_10D();
	virtual void		Unk_10E();
	virtual void		Unk_10F();
	virtual void		Unk_110();
	virtual void		Unk_111();
	virtual void		Unk_112();
	virtual void		Unk_113();
	virtual void		Unk_114();
	virtual void		Unk_115();
	virtual void		Unk_116();
	virtual void		Unk_117();
	virtual void		Unk_118();
	virtual void		Unk_119();
	virtual void		Unk_11A();
	virtual void		Unk_11B();
	virtual void		Unk_11C();
	virtual void		Unk_11D();
	virtual void		Unk_11E();
	virtual void		Unk_11F();
	virtual void		Unk_120();
	virtual void		Unk_121();
	virtual void		Unk_122();	//	0122	Check for LevelUp
	virtual void		Unk_123();
	virtual void		Unk_124();
	virtual void		Unk_125();
	virtual void		SetPerkRank(BGSPerk* perk, UInt8 rank, bool alt);
	virtual void		Unk_127();
	virtual UInt8		GetPerkRank(BGSPerk* perk, bool alt);
	virtual void		Unk_129();
	virtual void		Unk_12A();
	virtual void		Unk_12B();
	virtual void		Unk_12C();
	virtual void		Unk_12D();
	virtual void		Unk_12E();
	virtual void		Unk_12F();
	virtual void		Unk_130();
	virtual void		Unk_131();
	virtual void		Unk_132();
	virtual void		Unk_133();
	virtual void		Unk_134();
	virtual void		Unk_135();
	virtual void		Unk_136();

	struct Disposition {
		UInt32 disposition;
		UInt32 target;
	};

	typedef BSSimpleList<Disposition*> DispositionList;

	struct ActorValueList : BSSimpleList<UInt32> {
		bool	unk;
		UInt32	unk2;
	};

	bhkRagdollController*		pRagdollController;
	bhkRagdollPenetrationUtil*	pRagdollPenetrationUtil;
	UInt32						unk0B4;
	float						flt0B8;
	bool						bIsAIProcessing;
	UInt8 byte0BD;
	UInt8 byte0BE;
	UInt8 byte0BF;
	Actor*						pKiller;
	UInt8 byte0C4;
	UInt8 byte0C5;
	UInt8 byte0C6;
	UInt8 byte0C7;
	float time0C8;
	float					fCheckDeadBodyTimer;
	ActorValueList			kForceAVList;
	ActorValueList			kSetAVList;
	UInt8 byte0F0;
	UInt8 byte0F1;
	UInt8 byte0F2;
	UInt8 byte0F3;
	BSSimpleList<UInt32>	list0F4;
	DispositionList			dispositions;
	bool					bIsInCombat;
	UInt8					jipActorFlags1;
	UInt8					jipActorFlags2;
	UInt8					jipActorFlags3;
	UInt32					uiLifeState;
	UInt32					uiCriticalStage;
	UInt32					uiQueuedAttackAnim;
	float					fPainSoundTimer;
	UInt8 byte118;
	UInt8 byte119;
	UInt16 jip11A;
	UInt32 flags11C;
	UInt32 hoursPassed120;
	bool bForceRun;
	bool bForceSneak;
	UInt8 byte126;
	UInt8 hasSearch;
	Actor* combatTarget;
	Actor** combatTargets;
	Actor** combatAllies;
	UInt8 byte134;
	UInt8 byte135;
	UInt16 jip136;
	UInt32 unk138;
	UInt32 minorCrimeCount;
	UInt32 actorFlags;
	bool ignoreCrime;
	UInt8 byte145;
	UInt8 isResetAI;
	UInt8 byte147;
	TESActorBase* actorbase148;
	UInt8 inWater;
	UInt8 isSwimming;
	UInt8 byte14E;
	UInt8 gap14F;
	UInt32 iActionValue;
	float fTimerOnAction;
	float fUpdateTargetTimer;
	UInt8 byte15C;
	UInt8 bShouldRotateToTrack;
	UInt16 jip15E;
	NiPoint3 startingPos;
	float flt16C;
	TESForm* startingWorldOrCell;
	UInt8 byte174;
	UInt8 byte175;
	UInt16 jip176;
	float fCachedSpread_Projectile;
	float fCachedSpread_Scoped;
	float fCachedSpread_WeaponModel;
	float fCachedSpread_VATS;
	float flt188;
	UInt8 isCameraRagdollAnimDist;
	bool bIsTeammate;
	UInt8 byte18E;
	UInt8 byte18F;
	ActorMover*					pActorMover;
	UInt32						unk194;
	TESPackage*					package198;
	float						fOneMinusRadResist;
	ContinuousBeamProjectile*	pContinuousBeamProjectile;
	UInt32 speechExpression;
	UInt32 emotionValue;
	UInt32 sitSleepState;
	bool bIsTurret;
	bool bForceHit;
	bool byte1B2;
	bool byte1B3;

	// OBSE: unk1 looks like quantity, usu. 1; ignored for ammo (equips entire stack). In NVSE, pretty much always forced internally to 1
	// OBSE: itemExtraList is NULL as the container changes entry is not resolved before the call
	// NVSE: Default values are those used by the vanilla script functions.
	void EquipItem(TESForm* objType, UInt32 equipCount = 1, ExtraDataList* itemExtraList = NULL, UInt32 unk3 = 1, bool lockEquip = false, UInt32 unk5 = 1);	// unk3 apply enchantment on player differently
	void UnequipItem(TESForm* objType, UInt32 unequipCount = 1, ExtraDataList* itemExtraList = NULL, UInt32 unk3 = 1, bool lockEquip = false, UInt32 unk5 = 1);
	SInt32 GetCurrentAction() const;
	bool IsInReloadAnim() const;
	bool IsWeaponOut() const;
	bool IsAiming() const;
	bool HasExtraRefractionProperty();
	bool IsGhost() const;
	bool CastsShadows() const;

	EquippedItemsList	GetEquippedItems();
	ExtraContainerDataArray	GetEquippedEntryDataList();
	ExtraContainerExtendDataArray GetEquippedExtendDataList();
	TESObjectWEAP* GetEquippedWeapon();
};

ASSERT_SIZE(Actor, 0x1B4);
ASSERT_OFFSET(Actor, pRagdollController, 0xAC);
ASSERT_OFFSET(Actor, pActorMover, 0x190);
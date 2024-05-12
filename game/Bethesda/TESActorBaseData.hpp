#pragma once

#include "BaseFormComponent.hpp"
#include "BSSimpleList.hpp"

class BGSVoiceType;
class TESFaction;

class TESActorBaseData : public BaseFormComponent {
public:
	TESActorBaseData();
	~TESActorBaseData();

	virtual void			Fn_04(TESForm* selectedForm);	// Called during form initialization after LoadForm and InitForm
	// flags access
	virtual bool			Fn_05();	// 00100000
	virtual bool			Fn_06();	// 00200000
	virtual bool			Fn_07();	// 10000000
	virtual bool			Fn_08();	// 20000000
	virtual bool			Fn_09();	// 80000000
	virtual bool			Fn_0A();	// 00400000
	virtual bool			Fn_0B();	// 00400000
	virtual bool			Fn_0C();	// 00800000
	virtual bool			Fn_0D();
	virtual bool			Fn_0E();
	virtual bool			Fn_0F();
	virtual bool			Fn_10();
	virtual bool			Fn_11();
	virtual bool			Fn_12();
	virtual void			Fn_13(void* arg);
	virtual bool			Fn_14();
	virtual void			Fn_15(void* arg);
	virtual UInt32			Fn_16();
	virtual void			Fn_17(void* arg);
	virtual UInt32			Fn_18();	// return unk08
	virtual float			Fn_19();	// return unk14
	virtual BGSVoiceType*	GetVoiceType();

	enum
	{
		kFlags_Female					= 1 << 0,
		kFlags_CreatureBiped			= kFlags_Female,
		kFlags_Essential				= 1 << 1,
		kFlags_HasCharGenFace			= 1 << 2,
		kFlags_CreatureWeaponAndShield	= kFlags_HasCharGenFace,
		kFlags_Respawn					= 1 << 3,
		kFlags_AutoCalcStats			= 1 << 4,
		kFlags_CreatureSwims			= kFlags_AutoCalcStats,
		kFlags_CreatureFlies			= 1 << 5,
		kFlags_CreatureWalks			= 1 << 6,
		kFlags_PCLevelMult				= 1 << 7,
		kFlags_UseTemplate				= 1 << 8,
		kFlags_NoLowLevelProcessing		= 1 << 9,
		//								1 << 10,
		kFlags_NoBloodSpray				= 1 << 11,
		kFlags_NoBloodDecal				= 1 << 12,
		//								1 << 13,
		//								1 << 14,
		kFlags_CreatureNoHead			= 1 << 15,
		kFlags_CreatureNoRightArm		= 1 << 16,
		kFlags_CreatureNoLeftArm		= 1 << 17,
		kFlags_CreatureNoCombatInWater	= 1 << 18,
		kFlags_CreatureNoShadow			= 1 << 19,
		kFlags_NoVATSMelee				= 1 << 20,
		kFlags_CreatureAllowPCDialogue	= 1 << 21,
		kFlags_CanBeAllRaces			= 1 << 22,
		kFlags_CreatureCantOpenDoor		= kFlags_CanBeAllRaces,
		kFlags_CreatureImmobile			= 1 << 23,
		kFlags_CreatureTiltFrontBack	= 1 << 24,
		kFlags_CreatureTiltLeftRight	= 1 << 25,
		kFlags_NoKnockdowns				= 1 << 26,
		kFlags_NotPushable				= 1 << 27,
		kFlags_CreatureAllowPickpocket	= 1 << 28,
		kFlags_CreatureIsGhost			= 1 << 28,
		kFlags_NoRotateToHeadTrack		= 1 << 30,
		kFlags_CreatureInvulnerable		= 1 << 31,
	};

	enum
	{
		kTemplateFlag_UseTraits = 1 << 0,
		kTemplateFlag_UseStats = 1 << 1,
		kTemplateFlag_UseFactions = 1 << 2,
		kTemplateFlag_UseActorEffecBSSimpleList = 1 << 3,
		kTemplateFlag_UseAIData = 1 << 4,
		kTemplateFlag_UseAIPackages = 1 << 5,
		kTemplateFlag_UseModelAnimation = 1 << 6,
		kTemplateFlag_UseBaseData = 1 << 7,
		kTemplateFlag_UseInventory = 1 << 8,
		kTemplateFlag_UseScript = 1 << 9,
	};

	struct FactionListData {
		TESFaction* faction;
		UInt8		rank;
		UInt8		pad[3];
	};

	Bitfield32		flags;				// 04	Comparing with LoadForm and FNVEdit
	UInt16			fatigue;			// 08	Fatique
	UInt16			barterGold;			// 0A	Barter Gold
	SInt16			level;				// 0C	Level/ Level Mult
	UInt16			calcMin;			// 0E	Calc min
	UInt16			calcMax;			// 10	Calc max
	UInt16			speedMultiplier;	// 12	Speed Multiplier (confirmed)
	float			Karma;				// 14	Karma
	UInt16			dispositionBase;	// 18	Disposition Base
	UInt16			templateFlags;		// 1A	Template Flags
	TESForm*		deathItem;		// 1C	Death Item: object or FormList
	BGSVoiceType*	voiceType;		// 20
	TESForm*		templateActor;		// 24	Points toward Template
#ifdef RUNTIME
	UInt32			changedFlags;		// 28/000	Absent in Editor
#endif
	BSSimpleList<FactionListData*>	factionList;	// 2C/28

	SInt8 GetFactionRank(TESFaction* faction);

	bool IsFemale() const { return flags.GetBit(kFlags_Female); }
};

ASSERT_SIZE(TESActorBaseData, 0x34);
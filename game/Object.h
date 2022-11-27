#pragma once
#include <Form.h>

class TESObject : public TESForm
{
public:
	TESObject();
	~TESObject();

	virtual UInt32	Unk_4E(void);
	virtual bool	Unk_4F(void);
	virtual UInt32	Unk_50(void);
	virtual bool	Unk_51(void);
	virtual void	Unk_52(void* arg);
	virtual NiNode* _CreateNiNode(TESObjectREFR* refr, void* arg1);
	virtual void	Unk_54(void* arg);
	virtual bool	IsInternal(void);
	virtual bool	IsInternalMarker(void);
	virtual void	Unk_57(void);
	virtual bool	Unk_58(void);	// BoundObject: Calls Unk_5F on the object model
	virtual bool	Unk_59(void* arg);
	virtual void	Unk_5A(void* arg0, void* arg1);
	virtual UInt32	Unk_5B(void);
	virtual UInt32	Unk_5C(void);
	virtual bool	__CreateNiNode(TESObjectREFR* refr);	// if false, no NiNode gets returned by Unk_53, true for NPC
};

// 30
class TESBoundObject : public TESObject
{
public:
	TESBoundObject();
	~TESBoundObject();

	virtual NiNode* CreateNiNode(TESObjectREFR* refr);	// calls Fn53, for NPC calls ReadBones, for LevelledActor level them if necessary
	virtual bool	Unk_5F(void);

	BoundObjectListHead* head;		// 018
	TESBoundObject* prev;		// 01C
	TESBoundObject* next;		// 020

	struct NiPoint3ForPoor
	{
		SInt16 x, y, z;
	};
	union
	{
		struct {
			NiPoint3ForPoor		left;
			NiPoint3ForPoor		right;
		};
		SInt16 bounds[6];
	};
};
static_assert(sizeof(TESBoundObject) == 48);


// 30
class TESBoundAnimObject : public TESBoundObject
{
public:
	TESBoundAnimObject();
	~TESBoundAnimObject();
};


// 10C
class TESActorBase : public TESBoundAnimObject
{
public:
	TESActorBase();
	~TESActorBase();

	virtual BGSBodyPartData* GetBodyPartData(void);
	virtual void				Fn_61(void* arg);
	virtual TESCombatStyle* GetCombatStyle(void);	// Result saved as ZNAM GetCombatStyle
	virtual void				SetCombatStyle(TESCombatStyle* combatStyle);
	virtual void				SetAttr(UInt32 idx, float value);	// calls Fn65
	virtual void				SetAttr(UInt32 idx, UInt32 value);
	virtual void				ModActorValue(UInt32 actorValueCode, float value);
	virtual void				Fn_67(void* arg0, UInt32 arg1);	// mod actor value?

	// bases
	TESActorBaseData			baseData;		// 030/054
	TESContainer				container;		// 064/084
	BGSTouchSpellForm			touchSpell;		// 070/090	Unarmed attack effect
	TESSpellList				spellList;		// 07C/09C
	TESAIForm					ai;				// 090
	TESHealthForm				health;			// 0B0
	TESAttributes				attributes;		// 0B8
	TESAnimation				animation;		// 0C4/0E4	special idles
	TESFullName					fullName;		// 0D0/0F8
	TESModel					model;			// 0DC/104
	TESScriptableForm			scriptable;		// 0F4/128
	ActorValueOwner				avOwner;		// 100
	BGSDestructibleObjectForm	destructible;	// 104
	// 10C
};
static_assert(sizeof(TESActorBase) == 0x10C);


// A0
class BGSTextureSet : public TESBoundObject
{
public:
	BGSTextureSet();
	~BGSTextureSet();

	enum	// texture types
	{
		kDiffuse = 0,
		kNormal,
		kEnvMask,
		kGlow,
		kParallax,
		kEnv
	};

	enum
	{
		kTexFlag_NoSpecMap = 0x0001,
	};

	// 24
	struct DecalInfo
	{
		enum
		{
			kFlag_Parallax = 0x01,
			kFlag_AlphaBlend = 0x02,
			kFlag_AlphaTest = 0x04,
		};

		float	minWidth;		// 00
		float	maxWidth;		// 04
		float	minHeight;		// 08
		float	maxHeight;		// 0C
		float	depth;			// 10
		float	shininess;		// 14
		float	parallaxScale;	// 18
		UInt8	parallaxPasses;	// 1C
		UInt8	flags;			// 1D
		UInt8	pad1E[2];		// 1E
		UInt32	color;			// 20
	};

	BSTextureSet	bsTexSet;		// 30

	TESTexture		textures[6];	// 38
	DecalInfo* decalInfo;	// 80
	UInt16			texFlags;		// 84
	UInt8			pad86[2];		// 86
	UInt32			unk88;			// 88
	UInt32			unk8C;			// 8C
	UInt32			unk90;			// 90
	UInt32			unk94;			// 94
	UInt32			unk98;			// 98
	UInt32			unk9C;			// 9C
};

static_assert(sizeof(BGSTextureSet) == 0xA0);


// 68
class TESSound : public TESBoundAnimObject
{
public:
	TESSound();
	~TESSound();

	enum
	{
		kFlag_RandomFrequencyShift = 1,
		kFlag_PlayAtRandom = 2,
		kFlag_EnvironmentIgnored = 4,
		kFlag_RandomLocation = 8,
		kFlag_Loop = 16,
		kFlag_MenuSound = 32,
		kFlag_2D = 64,
		kFlag_360LFE = 128,
		kFlag_DialogueSound = 256,
		kFlag_EnvelopeFast = 512,
		kFlag_EnvelopeSlow = 1024,
		kFlag_2DRadius = 2048,
		kFlag_MuteWhenSubmerged = 4096,
		kFlag_StartAtRandomPosition = 8192,
	};

	TESSoundFile	soundFile;				// 30

	UInt32			unk3C;					// 3C
	UInt16			unk40;					// 40
	UInt16			unk42;					// 42
	UInt8			minAttenuationDist;		// 44
	UInt8			maxAttenuationDist;		// 45
	SInt16			frequencyAdj;			// 46
	UInt32			soundFlags;				// 48
	UInt16			staticAttenuation;		// 4C
	UInt8			endsAt;					// 4E
	UInt8			startsAt;				// 4F
	UInt16			attenuationCurve[5];	// 50
	UInt16			reverbAttenuation;		// 5A
	UInt32			priority;				// 5C
	UInt32			unk60;					// 60
	UInt32			unk64;					// 64

	void SetFlag(UInt32 pFlag, bool bMod)
	{
		soundFlags = bMod ? (soundFlags | pFlag) : (soundFlags & ~pFlag);
	}
};
static_assert(sizeof(TESSound) == 0x68);

// 3C
class BGSAcousticSpace : public TESBoundObject
{
public:
	BGSAcousticSpace();
	~BGSAcousticSpace();

	UInt32	unk30;	// 30
	UInt32	unk34;	// 34
	UInt32	unk38;	// 38
};

static_assert(sizeof(BGSAcousticSpace) == 0x3C);

// 68
class TESGrass : public TESBoundObject
{
public:
	TESGrass();
	~TESGrass();

	TESModel		model;					// 30

	UInt8			density;				// 48
	UInt8			minSlope;				// 49
	UInt8			maxSlope;				// 4A
	UInt8			pad4B;					// 4B
	UInt16			unitFromWaterAmount;	// 4C
	UInt8			pad4E[2];				// 4E
	UInt8			unitFromWaterType;		// 50
	UInt8			pad51[3];				// 51
	float			positionRange;			// 54
	float			heightRange;			// 58
	float			colorRange;				// 5C
	float			wavePeriod;				// 60
	UInt8			flags;					// 64
	UInt8			pad65[3];				// 65
};

static_assert(sizeof(TESGrass) == 0x68);


class BGSTalkingActivator;

// TESObjectACTI (84)
class TESObjectACTI : public TESBoundAnimObject
{
public:
	TESObjectACTI();
	~TESObjectACTI();

	// bases
	TESFullName					fullName;			// 30
	TESModelTextureSwap			modelTextureSwap;	// 3C
	TESScriptableForm			scriptable;			// 5C
	BGSDestructibleObjectForm	destructible;		// 68
	BGSOpenCloseForm			openClose;			// 70

	TESSound* loopingSound;		// 74	SNAM
	TESSound* activationSound;	// 78	VNAM
	TESSound* radioTemplate;		// 7C	INAM
	TESWaterForm* waterType;			// 80	WNAM 
	BGSTalkingActivator* radioStation;		// 84	RNAM
	String						activationPrompt;	// 88	XATO
};

static_assert(sizeof(TESObjectACTI) == 0x90);

// BGSTalkingActivator (8C)
class BGSTalkingActivator;

class BGSNote;

// BGSTerminal (9C)
class BGSTerminal : public TESObjectACTI
{
public:
	BGSTerminal();
	~BGSTerminal();

	enum
	{
		kTerminalFlagLeveled = 1 << 0,
		kTerminalFlagUnlocked = 1 << 1,
		kTerminalFlagAltColors = 1 << 2,
		kTerminalFlagHideWelcome = 1 << 3,
	};

	enum
	{
		kEntryFlagAddNote = 1 << 0,
		kEntryFlagForceRedraw = 1 << 1,
	};

	struct TermData
	{
		UInt8 difficulty;       // 0: very easy, 1: easy, 2: average, 3: hard, 4: very hard, 5: requires key
		UInt8 terminalFlags;
		UInt8 type;             // 0-9, corresponds to GECK types 1-10
	};

	struct MenuEntry
	{
		String				entryText;
		String				resultText;
		UInt8				entryFlags;
		BGSNote* displayNote;
		BGSTerminal* subMenu;
		ScriptEventList* scriptEventList;
		TList<Condition*>	conditions;
	};

	String				desc;			// 090	DESC
	TList<MenuEntry>	menuEntries;	// 098
	BGSNote* password;		// 0A0	PNAM
	TermData			data;			// 0A4	DNAM
};

// 180
class TESObjectARMO : public TESBoundObject
{
public:
	TESObjectARMO();
	~TESObjectARMO();

	enum eBipedFlags {
		eBipedFlag_HasBackPack = 0x4,
		eBipedFlag_MediumArmor = 0x8,
		eBipedFlag_PowerArmor = 0x20,
		eBipedFlag_NonPlayable = 0x40,
		eBipedFlag_HeavyArmor = 0x80,
	};
	// children
	TESFullName					fullName;		// 030
	TESScriptableForm			scriptable;		// 03C
	TESEnchantableForm			enchantable;	// 048
	TESValueForm				value;			// 058
	TESWeightForm				weight;			// 060
	TESHealthForm				health;			// 068
	TESBipedModelForm			bipedModel;		// 070
	BGSDestructibleObjectForm	destuctible;	// 14C
	BGSEquipType				equipType;		// 154
	BGSRepairItemList			repairItemList;	// 15C
	BGSBipedModelList			bipedModelList; // 164
	BGSPickupPutdownSounds		unk16C;			// 16C
	UInt16						armorRating;	// 178
	UInt16						modifiesVoice;	// 17A
	float						damageThreshold;// 17C
	UInt32						armorFlags;		// 180
	UInt32						unk184;			// 184
	// 180

	UInt32						GetArmorValue(UInt32);
};
static_assert(sizeof(TESObjectARMO) == 0x188);

// TESObjectBOOK (C4)
class TESObjectBOOK;

// 154
class TESObjectCLOT : public TESBoundObject
{
public:
	TESObjectCLOT();
	~TESObjectCLOT();

	// bases
	TESFullName					fullName;		// 030
	TESScriptableForm			scriptable;		// 03C
	TESEnchantableForm			enchantable;	// 048
	TESValueForm				value;			// 058
	TESWeightForm				weight;			// 060
	TESBipedModelForm			bipedModel;		// 068
	BGSDestructibleObjectForm	destuctible;	// 144
	BGSEquipType				equipType;		// 14C
	// unk data
};

// TESObjectCONT (9C / D4)
class TESObjectCONT : public TESBoundAnimObject
{
public:
	TESObjectCONT();
	~TESObjectCONT();

	TESContainer                container;          // 30
	TESFullName					name;				// 3C
	TESModelTextureSwap			model;				// 48
	TESScriptableForm			scriptForm;			// 68
	TESWeightForm				weightForm;			// 74
	BGSDestructibleObjectForm	destructForm;		// 7C
	BGSOpenCloseForm			openCloseForm;		// 84 / B8

	UInt32						unk88;				// 088
	TESSound* openSound;						// 08C
	TESSound* closeSound;						// 090
	TESSound* randomLoopingSound;				// 094
	UInt8		flags;								// 098
	UInt8		pad099[3];
};
static_assert(sizeof(TESObjectCONT) == 0x9C);

// TESObjectDOOR (90)
class TESObjectDOOR : public TESBoundAnimObject
{
public:
	TESObjectDOOR();
	~TESObjectDOOR();

	TESFullName					name;				// 30
	TESModelTextureSwap			model;				// 3C
	TESScriptableForm			scriptForm;			// 5C
	//	TESMagicCasterForm			magicCaster;		// 
	//	TESMagicTargetForm			magicTarget;		// 
	BGSDestructibleObjectForm	destructForm;		// 68
	BGSOpenCloseForm			openCloseForm;		// 70
	// There is a TList in 088
};


// TESObjectLIGH (C8)
class TESObjectLIGH : public TESBoundAnimObject
{
public:
	TESObjectLIGH();
	~TESObjectLIGH();

	enum
	{
		kFlag_Dynamic = 1,
		kFlag_CanBeCarried = 2,
		kFlag_Negative = 4,
		kFlag_Flicker = 8,
		kFlag_Unused = 16,
		kFlag_OffByDefault = 32,
		kFlag_FlickerSlow = 64,
		kFlag_Pulse = 128,
		kFlag_PulseSlow = 256,
		kFlag_SpotLight = 512,
		kFlag_SpotShadow = 1024,
	};

	TESFullName					fullName;		// 030
	TESModelTextureSwap			modelSwap;		// 03C
	TESIcon						icon;			// 05C
	BGSMessageIcon				messageIcon;	// 068
	TESScriptableForm			scriptable;		// 078
	TESWeightForm				weight;			// 084
	TESValueForm				value;			// 08C
	BGSDestructibleObjectForm	destructible;	// 094

	SInt32						time;			// 09C
	UInt32						radius;			// 0A0
	UInt8						red;			// 0A4
	UInt8						green;			// 0A5
	UInt8						blue;			// 0A6
	UInt8						padA7;			// 0A7
	UInt32						lightFlags;		// 0A8
	float						falloffExp;		// 0AC
	float						FOV;			// 0B0
	float						fadeValue;		// 0B4
	TESSound* sound;			// 0B8
	UInt32						padBC[3];		// 0BC

	void SetFlag(UInt32 pFlag, bool bMod)
	{
		lightFlags = bMod ? (lightFlags | pFlag) : (lightFlags & ~pFlag);
	}
};
static_assert(sizeof(TESObjectLIGH) == 0x0C8);

// TESObjectMISC (A8)
class TESObjectMISC : public TESBoundObject
{
public:
	TESObjectMISC();
	~TESObjectMISC();

	UInt32	unk30[30];
};

// 50
class TESObjectSTAT : public TESBoundObject
{
public:
	TESObjectSTAT();
	~TESObjectSTAT();

	// children
	TESModel	model;	// 
};


class TESObjectIMOD : public TESBoundObject
{
public:
	TESObjectIMOD();
	~TESObjectIMOD();

	// bases
	TESFullName					name;				// 030
	TESModelTextureSwap			model;				// 03C
	TESIcon						icon;				// 05C
	TESScriptableForm			scriptForm;			// 068
	TESDescription				description;		// 074
	TESValueForm				value;				// 07C
	TESWeightForm				weight;				// 084
	BGSDestructibleObjectForm	destructible;		// 08C
	BGSMessageIcon				messageIcon;		// 094
	BGSPickupPutdownSounds		pickupPutdownSounds;// 0A4
};

// 388
class TESObjectWEAP : public TESBoundObject
{
public:
	TESObjectWEAP();
	~TESObjectWEAP();
	bool IsMeleeWeapon() const;
	float GetModBonuses(UInt32 effectID);

	enum EWeaponType {
		kWeapType_HandToHandMelee = 0,
		kWeapType_OneHandMelee,
		kWeapType_TwoHandMelee,
		kWeapType_OneHandPistol,
		kWeapType_OneHandPistolEnergy,
		kWeapType_TwoHandRifle,
		kWeapType_TwoHandAutomatic,
		kWeapType_TwoHandRifleEnergy,
		kWeapType_TwoHandHandle,
		kWeapType_TwoHandLauncher,
		kWeapType_OneHandGrenade,
		kWeapType_OneHandMine,
		kWeapType_OneHandLunchboxMine,
		kWeapType_OneHandThrown,
		kWeapType_Last	// During animation analysis, player weapon can be classified as 0x0C = last
	};

	enum EWeaponSounds {
		kWeapSound_Shoot3D = 0,
		kWeapSound_Shoot2D,
		kWeapSound_Shoot3DLooping,
		kWeapSound_NoAmmo,
		kWeapSound_Swing = kWeapSound_NoAmmo,
		kWeapSound_Block,
		kWeapSound_Idle,
		kWeapSound_Equip,
		kWeapSound_Unequip
	};

	enum EHandGrip {
		eHandGrip_Default	= 0xFF,
		eHandGrip_1			= 0xE6,
		eHandGrip_2			= 0xE7,
		eHandGrip_3			= 0xE8,
		eHandGrip_4			= 0xE9,
		eHandGrip_5			= 0xEA,
		eHandGrip_6			= 0xEB,
		eHandGrip_Count		= 7,
	};

	enum EAttackAnimations {
		eAttackAnim_Default			= 0xff,	// 11111111
		eAttackAnim_Attack3			= 0x26,	// 00100110
		eAttackAnim_Attack4			= 0x2c, // 00101100
		eAttackAnim_Attack5			= 0x32,	// 00110010
		eAttackAnim_Attack6			= 0x38, // 
		eAttackAnim_Attack7			= 0x3e,
		eAttackAnim_Attack8			= 0x44,
		eAttackAnim_AttackLeft		= 0x1a,
		eAttackAnim_AttackLoop		= 0x4a,
		eAttackAnim_AttackRight		= 0x20,
		eAttackAnim_AttackSpin		= 0x50,
		eAttackAnim_AttackSpin2		= 0x56,
		eAttackAnim_AttackThrow		= 0x6d,
		eAttackAnim_AttackThrow2	= 0x73,
		eAttackAnim_AttackThrow3	= 0x79,
		eAttackAnim_AttackThrow4	= 0x7F,
		eAttackAnim_AttackThrow5	= 0x85,
		eAttackAnim_PlaceMine		= 0x61,
		eAttackAnim_PlaceMine2		= 0x67,
		eAttackAnim_Count			= 19,
	};

	enum ReloadAnim {
		eReload_A = 0,
		eReload_B,
		eReload_C,
		eReload_D,
		eReload_E,
		eReload_F,
		eReload_G,
		eReload_H,
		eReload_I,
		eReload_J,
		eReload_K,
		eReload_L,
		eReload_M,
		eReload_N,
		eReload_O,
		eReload_P,
		eReload_Q,
		eReload_R,
		eReload_S,
		eReload_W,
		eReload_X,
		eReload_Y,
		eReload_Z,
		eReload_Count,
	};
	static_assert(eReload_Count == 23);

	enum EWeaponFlags1 {
		eFlag_IgnoresNormalWeapResist	= 0x1,
		eFlag_IsAutomatic				= 0x2,
		eFlag_HasScope					= 0x4,
		eFlag_CantDrop					= 0x8,
		eFlag_HideBackpack				= 0x10,
		eFlag_EmbeddedWeapon			= 0x20,
		eFlag_No1stPersonISAnims		= 0x40,
		Eflag_NonPlayable				= 0x80
	};

	enum EWeaponFlags2 {
		eFlag_playerOnly				= 0x1,
		eFlag_NPCsUseAmmo				= 0x2,
		eFlag_NoJamAfterReload			= 0x4,
		eFlag_ActionPointOverride		= 0x8,
		eFlag_MinorCrime				= 0x10,
		eFlag_FixedRange				= 0x20,
		eFlag_NotUsedNormalCombat		= 0x40,
		eFlag_DamageToWeaponOverride	= 0x80,
		eFlag_No3rdPersonISAnims		= 0x100,
		eFlag_BurstShot					= 0x200,
		eFlag_RumbleAlternate			= 0x400,
		eFlag_LongBurst					= 0x800,
		eFlag_NightVision				= 0x1000,
		eFlag_ScopeFromMod				= 0x2000,
	};

	enum EEmbedWeapAV {
		eEmbedAV_Perception				= 0,
		eEmbedAV_Endurance,
		eEmbedAV_LeftAttack,
		eEmbedAV_RightAttack,
		eEmbedAV_LeftMobility,
		eEmbedAV_RightMobility,
		eEmbedAV_Brain,
	};

	enum EOnHit {
		eOnHit_Normal					= 0,
		eOnHit_DismemberOnly,
		eOnHit_ExplodeOnly,
		eOnHit_NoDismemberOrExplode,
	};

	enum ERumblePattern {
		eRumblePattern_Constant			= 0,
		eRumblePattern_Square,
		eRumblePattern_Triangle,
		eRumblePattern_Sawtooth
	};

	enum ECritDamageFlags {
		eCritDamage_OnDeath				= 0x1
	};

	enum kWeaponModEffects
	{
		kWeaponModEffect_None = 0,
		kWeaponModEffect_IncreaseDamage,
		kWeaponModEffect_IncreaseClipCapacity,
		kWeaponModEffect_DecreaseSpread,
		kWeaponModEffect_DecreaseWeight,
		kWeaponModEffect_Unused05,
		kWeaponModEffect_RegenerateAmmo,
		kWeaponModEffect_DecreaseEquipTime,
		kWeaponModEffect_IncreaseRateOfFire,		// 8
		kWeaponModEffect_IncreaseProjectileSpeed,
		kWeaponModEffect_IncreaseMaxCondition,
		kWeaponModEffect_Silence,
		kWeaponModEffect_SplitBeam,
		kWeaponModEffect_VATSBonus,
		kWeaponModEffect_IncreaseZoom,				// 14
	};

	enum EWeapValues
	{
		eWeap_Type = 0,
		eWeap_MinSpread,
		eWeap_Spread,
		eWeap_Proj,
		eWeap_SightFOV,
		eWeap_MinRange,
		eWeap_Range,
		eWeap_AmmoUse,
		eWeap_APCost,
		eWeap_CritDam,
		eWeap_CritChance,
		eWeap_CritEffect,
		eWeap_FireRate,
		eWeap_AnimAttackMult,
		eWeap_RumbleLeft,
		eWeap_RumbleRight,
		eWeap_RumbleDuration,
		eWeap_RumbleWaveLength,
		eWeap_AnimShotsPerSec,
		eWeap_AnimReloadTime,
		eWeap_AnimJamTime,
		eWeap_Skill,
		eWeap_ResistType,
		eWeap_FireDelayMin,
		eWeap_FireDelayMax,
		eWeap_AnimMult,
		eWeap_Reach,
		eWeap_IsAutomatic,
		eWeap_HandGrip,
		eWeap_ReloadAnim,
		eWeap_VATSChance,
		eWeap_AttackAnim,
		eWeap_NumProj,
		eWeap_AimArc,
		eWeap_LimbDamageMult,
		eWeap_SightUsage,
		eWeap_ReqStr,
		eWeap_ReqSkill,
		eWeap_LongBursts,
		eWeap_Flags1,
		eWeap_Flags2,
		eWeap_HasScope,
		eWeap_IgnoresDTDR,
		eWeap_SoundLevel,
		eWeap_ClipSize,
	};
	
	// bases
	TESFullName					fullName;			// 030
	TESModelTextureSwap			textureSwap;		// 03C
	TESIcon						icon;				// 05C
	TESScriptableForm			scritpable;			// 068
	TESEnchantableForm			enchantable;		// 074
	TESValueForm				value;				// 084
	TESWeightForm				weight;				// 08C
	TESHealthForm				health;				// 094
	TESAttackDamageForm			attackDmg;			// 09C
	BGSAmmoForm					ammo;				// 0A4
	BGSClipRoundsForm			clipRounds;			// 0AC
	BGSDestructibleObjectForm	destructible;		// 0B4
	BGSRepairItemList			repairItemList;		// 0BC
	BGSEquipType				equipType;			// 0C4
	BGSPreloadable				preloadable;		// 0CC
	BGSMessageIcon				messageIcon;		// 0D0
	BGSBipedModelList			bipedModelList;		// 0E0
	BGSPickupPutdownSounds		pickupPutdownSounds;// 0E8

	UInt8				eWeaponType;			// 0F4
	UInt8				pad[3];
	float				animMult;				// 0F8
	float				reach;					// 0FC
	UInt8				weaponFlags1;			// 100
	UInt8				handGrip;				// 101
	UInt8				ammoUse;				// 102
	UInt8				reloadAnim;				// 103
	float				minSpread;				// 104
	float				spread;					// 108
	UInt32				unk10C;					// 10C
	float				sightFOV;				// 110
	UInt32				unk114;					// 114
	BGSProjectile*		projectile;				// 118
	UInt8				baseVATSChance;			// 11C
	UInt8				attackAnim;				// 11D
	UInt8				numProjectiles;			// 11E
	UInt8				embedWeaponAV;			// 11F
	float				minRange;				// 120
	float				maxRange;				// 124
	UInt32				onHit;					// 128
	UInt32				weaponFlags2;			// 12C
	float				animAttackMult;			// 130
	float				fireRate;				// 134
	float				AP;						// 138
	float				rumbleLeftMotor;		// 13C
	float				rumbleRightMotor;		// 140
	float				rumbleDuration;			// 144
	float				damageToWeaponMult;		// 148
	float				animShotsPerSec;		// 14C
	float				animReloadTime;			// 150
	float				animJamTime;			// 154		
	float				aimArc;					// 158
	UInt32				weaponSkill;			// 15C - actor value
	UInt32				rumblePattern;			// 160 - reload anim?
	float				rumbleWavelength;		// 164
	float				limbDamageMult;			// 168
	UInt32				resistType;				// 16c - actor value
	float				sightUsage;				// 170
	float				semiAutoFireDelayMin;	// 174
	float				semiAutoFireDelayMax;	// 178
	UInt32				unk17C;					// 17C - 0-0x10: 0x8:str req 0x10: - skill req  - 0xb:kill impulse B158 - mod 1 val B15C - Mod 2 val Effects: 0x1: e(zoom) 0x2: a 0x3:0 0x4-6: Values c-e Mod Effects Val2:1-3 
	UInt32				effectMods[3];			// 180
	float				value1Mod[3];			// 18C
	UInt32				powerAttackAnimOverride;	// 198
	UInt32				strRequired;			// 19C
	UInt8				unk1A0;					// 1A0
	UInt8				modReloadAnim;			// 1A1
	UInt8				pad1A2[2];				// 1A2
	float				regenRate;				// 1A4
	float				killImpulse;			// 1A8
	float				value2Mod[3];			// 1AC
	float				impulseDist;			// 1B8
	UInt32				skillRequirement	;	// 1BC
	UInt32				criticalDamage;			// 1C0
	float				criticalPercent;		// 1C4
	UInt8				critDamageFlags;		// 1C8
	UInt8				pad1C9[3];				// 1C9
	SpellItem*			criticalEffect;		// 1CC
	TESModel			shellCasingModel;		// 1DO
	TESModel			targetNIF;				// 1E8 - target NIF
	TESModel			model200;				// 200 - could be a texture swap
	UInt32				unk218;					// 218
	TESSound*			shoot3DSound;
	TESSound*			shootDistSound;
	TESSound*			shoot2DSound;
	TESSound*			soundShoot3DLooping;
	TESSound*			noAmmoSound;
	TESSound*			blockSound;
	TESSound*			idleSound;
	TESSound*			equipSound;
	TESSound*			unequipSound;
	TESSound*			modShoot3DSound;
	TESSound*			modShootDistSound;
	TESSound*			modShoot2DSound;
	BGSImpactDataSet*	impactDataSet;			// 24C
	TESObjectSTAT*		worldStatic;			// 250
	TESObjectSTAT*		mod1Static;				// 254
	TESObjectSTAT*		mod2Static;				// 258
	TESObjectSTAT*		mod3Static;				// 25C
	TESObjectSTAT*		mod12Static;			// 260
	TESObjectSTAT*		mod13Static;			// 264
	TESObjectSTAT*		mod23Static;			// 268
	TESObjectSTAT*		mod123Static;			// 26C
	TESModelTextureSwap	textureMod1;			// 270 Mod 1
	TESModelTextureSwap	textureMod2;			// 290 Mod 2
	TESModelTextureSwap	textureMod3;			// 2B0 Mod 3
	TESModelTextureSwap	textureMod12;			// 2D0 Mod 1-2
	TESModelTextureSwap	textureMod13;			// 2F0 Model 1-3
	TESModelTextureSwap	textureMod23;			// 310 Model 2-3
	TESModelTextureSwap	textureMod123;			// 330 Model 1-2-3
	TESObjectIMOD*		itemMod1;				// 350
	TESObjectIMOD*		itemMod2;				// 354
	TESObjectIMOD*		itemMod3;				// 358
	UInt32				unk35C;					// 35C
	UInt32				unk360;					// 360
	UInt32				soundLevel;				// 364
	UInt32				unk368;					// 368
	UInt32				unk36C;					// 36C
	UInt32				unk370;					// 370
	UInt32				unk374;					// 374
	UInt32				unk378;					// 378
	UInt32				unk37C;					// 37C
	UInt32				recharge;				// 380 maybe recharge
	UInt8				unk384;					// 384
	UInt8				unk385[3];				// 385


	bool					IsAutomatic() const { return weaponFlags1 & (eFlag_IsAutomatic); }
	void					SetIsAutomatic(bool bAuto) { bAuto ? weaponFlags1 |= eFlag_IsAutomatic : weaponFlags1 &= ~eFlag_IsAutomatic; }

	bool					HasScope() const { return weaponFlags1 & (eFlag_HasScope); }
	bool					IsNonPlayable() { return weaponFlags1 & ( 0x80 ); }
	UInt8					HandGrip() const;
	void					SetHandGrip(UInt8 handGrip);
	UInt8					AttackAnimation() const;
	void					SetAttackAnimation(UInt8 attackAnim);
	TESObjectIMOD*			GetItemMod(UInt8 which);
	__forceinline UInt32	GetItemModEffect(UInt8 which)	{ which -= 1; assert(which < 3); return effectMods[which]; }
	__forceinline Float32	GetItemModValue1(UInt8 which)	{ which -= 1; assert(which < 3); return value1Mod[which]; }
	__forceinline Float32	GetItemModValue2(UInt8 which)	{ which -= 1; assert(which < 3); return value2Mod[which]; }
	Float32					GetEffectModValue(kWeaponModEffects value, UInt8 second = 0);

	void					SetPlayable(bool bAuto) { bAuto ? weaponFlags1 |= Eflag_NonPlayable : weaponFlags1 &= ~Eflag_NonPlayable; }
	bool					HasNightVision() const { return weaponFlags2 & eFlag_NightVision; }
	bool					IsMeleeWeapon();
	bool					IsRangedWeapon();
	void					SetAttackAnimation(UInt32 attackAnim);
	TESAmmo*				GetAmmo();
	__forceinline int		GetClipRounds(bool hasExtendedClipMod) { return ThisCall<int>(0x4FE160, this, hasExtendedClipMod); };
	bool					CanEquipAmmo(TESAmmo* ammo);
	bool					IsThrownWeapon() { return eWeaponType >= kWeapType_OneHandGrenade; };
	BGSListForm*			GetAmmoList();
	__forceinline bool		IgnoresDTDR() const { return weaponFlags1 & eFlag_IgnoresNormalWeapResist; }

	float					GetWeaponValue(UInt32 whichVal);
	__forceinline bool		HasScopeAlt() const { return weaponFlags1 & (eFlag_HasScope) && !(weaponFlags2 & eFlag_ScopeFromMod); }
	__forceinline TESAmmo*	GetEquippedAmmo(Actor* actor) { return ThisCall<TESAmmo*>(0x525980, this, actor); }
};
static_assert(sizeof(TESObjectWEAP) == 0x388);


// DC
class TESAmmo : public TESBoundObject
{
public:
	TESAmmo();
	~TESAmmo();

	enum eAmmoFlags
	{
		kFlags_IgnoreWeapResistance =	1,
		kFlags_NonPlayable =			2,
	};

	// bases
	TESFullName					fullName;				// 030
	TESModelTextureSwap			model;					// 03C
	TESIcon						icon;					// 05C
	BGSMessageIcon				messageIcon;			// 068	
	TESValueForm				value;					// 078
	BGSClipRoundsForm			clipRounds;				// 080
	BGSDestructibleObjectForm	destructible;			// 088
	BGSPickupPutdownSounds		pickupPutdownsounds;	// 090
	TESScriptableForm			scriptable;				// 09C

	float						speed;					// 0A8	8 bytes makes it DATA
	UInt32						flags;					// 0AC	only 8 bits most likely. 0 is Ignores Normal Weapon Resistance', 1 is Non-Playable
	UInt32						projPerShot;			// 0B0
	BGSProjectile				* projectile;			// 0B4
	float						weight;					// 0B8
	TESObjectMISC				* casing;				// 0BC
	float						ammoPercentConsumed;	// 0C0
	String						shortName;				// 0C4
	String						abbreviation;			// 0CC
	TList<TESAmmoEffect>		effectList;				// 0D4

	bool IsNonPlayable() { return (flags & kFlags_NonPlayable) == kFlags_NonPlayable; }
	bool IsPlayable() { return !IsNonPlayable(); }
	SInt32 GetEffectIndex(TESAmmoEffect *effect);
};

static_assert(sizeof(TESAmmo) == 0xDC);


// 1EC
class TESNPC : public TESActorBase
{
public:
	TESNPC();
	~TESNPC();

	enum eFaceGenDataType {
		kFaceGenData_GeometrySymetric = 0,
		kFaceGenData_GeometryAsymetric = 0,
		kFaceGenData_TextureSymetric = 0,

	};

	// 20 - facegen data
	struct Data
	{
		void	* unk000;
		void	* unk004;	// used as this owning unk000.
		UInt32	unk008;
		void	* unk00C;	// the last three looks a lot like an array.
		UInt32	unk010;
		UInt32	unk014;
		UInt32 unk018[(0x058 - 0x018) >> 2];	// used as an array of 88 bytes
	};	// the owner of the array is FaceGenData::Unk008

	struct FaceGenData
	{						// sample Sunny Smiles
		UInt32	unk00;		// 000 134 filled		filled			filled
		void	* unk04;	// 004 138 null			null			null
		UInt32	unk08;		// 008 13C null			null			null
		float	** values;	// 00C 140 filled		filled			filled			base for offset to data. Looks like an array of arrays.
		UInt32	useOffset;	// 010 144 filled +0xC8	filled +0x78	filled +0xC8	participate in array base calculation
		UInt32	maxOffset;	// 014 148 = unk144		= unk144		= unk144
		UInt32  count;		// 018 14C 0x32			0x1e			0x32
		UInt32  size;		// 01C 150 0x01			0x01			0x01			also participate in array offset calculation (offset = count * size)
	};

	TESRaceForm	race;							// 10C/140
	UInt8		skillValues[0x0E];				// 114/148
	UInt8		skillOffsets[0x0E];				// 122
	TESClass	* classID;						// 130/164
	FaceGenData	faceGenData[3];					// 134/180	runtime suggests there are 2 arrays of 2 FaceGenData. the last one being null.
	UInt32		pad194[(0x1A4-0x194) >> 2];		// 194
	UInt32		unk1A4;
	UInt32		unk1A8;
	UInt32		unk1AC;
	UInt32		unk1B0;							// 1B0
	void		* unk1B4;						// 1B4	pointer to 84 bytes struct related to Face, can be returned instead of faceGenData so identical or at least starts the same ?
	TESHair		* hair;							// 1B8/1EC
	float		hairLength;						// 1BC/1F0
	TESEyes		* eyes;							// 1C0/1F4
	BSFaceGenNiNode	* unk1C4;					// 1C4
	BSFaceGenNiNode	* unk1C8;					// 1C8
	UInt32		unk1CC;							// 1CC
	UInt16		unk1D0;							// 1D0
	UInt16		unk1D2;							// 1D2
	TESCombatStyle	* unk1D4;					// 1D4
	UInt32		hairColor;						// 1D8/20C
	TList<BGSHeadPart>	headPart;				// 1DC/210
	UInt32		impactMaterialType;				// 1E4/218
	UInt32      unk01E8;						// 1E8
	TESRace     * race1EC;						// 1EC
	TESNPC      * copyFrom;						// 1F0		not set once PlayerRef exists and the target is the Player
	float		height;							// 1F4/21C
	float		weight;							// 1F8/220 Aparently, getWeight purposly returns height except for the player.
	NiTArray <FaceGenUndo *>	faceGenUndo;	// 1FC
};

static_assert(sizeof(TESNPC) == 0x20C);

// 160
class TESCreature : public TESActorBase
{
public:
	TESCreature();
	~TESCreature();

	TESAttackDamageForm			attackDmg;		// 10C
	TESModelList				modelList;		// 114

	// data
	TESCreature*				creatureTemplate;	// 128
	UInt8						type;				// 12C
	UInt8						combatSkill;		// 12D
	UInt8						magicSkill;			// 12E
	UInt8						stealthSkill;		// 12F
	UInt8						attackReach;		// 130
	UInt8						pad0131[3];			// 131
	float						turningSpeed;		// 134
	float						footWeight;			// 138
	float						baseScale;			// 13C
	TESCombatStyle*				combatStyle;		// 140
	BGSBodyPartData*			bodyPartData;		// 144
	UInt32						materialType;		// 148
	BGSImpactDataSet*			impactDataSet;		// 14C
	UInt32						unk0150;			// 150
	UInt32						soundLevel;			// 154
	BGSListForm*				meleeWeaponList;	// 158
	UInt8						byt015C;			// 15C
	UInt8						pad015D[3];			// 15D
};
static_assert(sizeof(TESCreature) == 0x160);


// TESLevCreature (68)
class TESLevCreature : public TESBoundObject
{
public:
	TESLevCreature();
	~TESLevCreature();

	TESLeveledList		list;		// 030
	TESModelTextureSwap	texture;	// 04C
};

// TESLevCharacter (68)
class TESLevCharacter : public TESBoundObject
{
public:
	TESLevCharacter();
	~TESLevCharacter();

	TESLeveledList		list;		// 030
	TESModelTextureSwap	texture;	// 04C
};

// TESKey (A8)
class TESKey : public TESObjectMISC
{
public:
	TESKey();
	~TESKey();
};

// D8
class AlchemyItem : public TESBoundObject
{
public:
	AlchemyItem();
	~AlchemyItem();

	MagicItem					magicItem;				// 30
	TESModelTextureSwap			model;					// 4C
	TESIcon						icon;					// 6C
	BGSMessageIcon				messageIcon;			// 78
	TESScriptableForm			scriptable;				// 88
	TESWeightForm				weight;					// 94
	BGSEquipType				equipType;				// 9C
	BGSDestructibleObjectForm	destructible;			// A4
	BGSPickupPutdownSounds		pickupPutdownsounds;	// AC
	UInt32						value;					// B8
	UInt8						alchFlags;				// BC
	UInt8						padBD[3];				// BD
	SpellItem*					withdrawalEffect;		// C0
	float						addictionChance;		// C4
	TESSound*					consumeSound;			// C8
	TESIcon						iconCC;					// CC

	bool IsPoison();
	bool RestoresAV(int avCode);
	UInt32 HasBaseEffectRestoresAV(SInt32);
	UInt32 HasBaseEffectDamagesAV(SInt32);
	bool IsAddictive();
	bool IsFood();
	bool IsMedicine();
	bool IsFoodAlt();
	bool IsWaterAlt();
};

static_assert(sizeof(AlchemyItem) == 0xD8);


// BGSNote (80)
class BGSNote : public TESBoundObject
{
public:
	BGSNote();
	~BGSNote();

	// bases
	TESModel					model;					// 030
	TESFullName					fullName;				// 048
	TESIcon						icon;					// 054
	BGSPickupPutdownSounds		pickupPutdownSounds;	// 060
	TESDescription				*noteText;				// 06C
	UInt32						unk070[3];				// 070
	UInt8                       unk07C;					// 07C
	UInt8                       read;					// 07D
	UInt8                       unk07E;					// 07E
	UInt8                       unk07F;					// 07F
};
static_assert(sizeof(BGSNote) == 0x80);


// 0C0
class BGSProjectile : public TESBoundObject
{
public:
	BGSProjectile();
	~BGSProjectile();

	enum
	{
		kFlags_Hitscan =				1 << 0,
		kFlags_Explosion =				1 << 1,
		kFlags_AltTrigger =				1 << 2,
		kFlags_MuzzleFlash =			1 << 3,
		//								1 << 4,	
		kFlags_CanBeDisabled =			1 << 5,
		kFlags_CanBePicked =			1 << 6,
		kFlags_Supersonic =				1 << 7,
		kFlags_PinsLimbs =				1 << 8,
		kFlags_PassSmallTransparent =	1 << 9,
		kFlags_Detonates =				1 << 10,
		kFlags_Rotation =				1 << 11,
	};

	TESFullName						fullName;			// 030
	TESModel						model;				// 03C
	BGSPreloadable					preloadable;		// 054
	BGSDestructibleObjectForm		destructible;		// 058

	UInt16							projFlags;			// 060
	UInt16							type;				// 062
	float							gravity;			// 064
	float							speed;				// 068
	float							range;				// 06C
	TESObjectLIGH*					lightProjectile;	// 070
	TESObjectLIGH*					lightMuzzleFlash;	// 074
	float							tracerChance;		// 078
	float							altProximity;		// 07C
	float							altTimer;			// 080
	BGSExplosion*					explosionForm;		// 084
	TESSound*						soundProjectile;	// 088
	float							flashDuration;		// 08C
	float							fadeDuration;		// 090
	float							impactForce;		// 094
	TESSound*						soundCountDown;		// 098
	TESSound*						soundDisable;		// 09C
	TESObjectWEAP*					defaultWeapSrc;		// 0A0
	float							rotationX;			// 0A4
	float							rotationY;			// 0A8
	float							rotationZ;			// 0AC
	float							bouncyMult;			// 0B0
	TESModel						muzzleFlash;		// 0B4
	UInt8							soundLevel;			// 0CC

	void SetFlag(UInt32 pFlag, bool bMod)
	{
		projFlags = bMod ? (projFlags | pFlag) : (projFlags & ~pFlag);
	}
};


// BGSExplosion (A4)
class BGSExplosion : public TESBoundObject
{
public:
	BGSExplosion();
	~BGSExplosion();

	enum
	{
		kFlags_Unknown						=	1 << 0,
		kFlags_AlwaysUseWorldOrientation	=	1 << 1,
		kFlags_KnockDownAlways				=	1 << 2,
		kFlags_KnockDownByFormula			=	1 << 3,
		kFlags_IgnoreLOSCheck				=	1 << 4,
		kFlags_PushSourceRefOnly			=	1 << 5,
		kFlags_IgnoreImageSpaceSwap			=	1 << 6,
	};

	TESFullName					fullName;			// 030
	TESModel					model;				// 03C
	TESEnchantableForm			enchantable;		// 054
	BGSPreloadable				preloadable;		// 064
	TESImageSpaceModifiableForm	imageSpaceModForm;	// 068

	TESForm*					placedObj;			// 070
	Float32						force;				// 074
	Float32						damage;				// 078
	Float32						radius;				// 07C
	TESObjectLIGH*				light;				// 080
	TESSound*					sound1;				// 084
	UInt32						explFlags;			// 088
	Float32						ISradius;			// 08C
	BGSImpactDataSet*			impactDataSet;		// 090
	TESSound*					sound2;				// 094
	Float32						RADlevel;			// 098
	Float32						dissipationTime;	// 09C
	Float32						RADradius;			// 0A0

	void SetFlag(UInt32 flag, bool val) { explFlags = val ? (explFlags | flag) : (explFlags & ~flag); }
};
static_assert(sizeof(BGSExplosion) == 0xA4);

class BGSAddonNode : public TESBoundObject
{
public:
	BGSAddonNode();
	~BGSAddonNode();

	TESModel	model;				// 030
	UInt32		unk48[(0x60 - 0x48) >> 2]; // 048
};
static_assert(sizeof(BGSAddonNode) == 0x60);

// TESObjectARMA (180)
class TESObjectARMA : public TESBoundObject
{
public:
	TESObjectARMA();
	~TESObjectARMA();

	TESFullName					fullName;				// 030
	TESScriptableForm			scriptable;				// 03C
	TESEnchantableForm			enchantable;			// 048
	TESValueForm				value;					// 058
	TESWeightForm				weight;					// 060
	TESHealthForm				health;					// 068
	TESBipedModelForm			bipedModelForm;			// 070
	BGSDestructibleObjectForm	destructible;			// 14C
	BGSEquipType				equipType;				// 154
	BGSRepairItemList			repairList;				// 15C
	BGSBipedModelList			bipedModelList;			// 164
	BGSPickupPutdownSounds		pickupPutdownSounds;	// 16C
	UInt32						unk178;					// 178			
	UInt32						unk17C;					// 17C
};
static_assert(sizeof(TESObjectARMA) == 0x180);

class TESCaravanCard : public TESBoundObject
{
public:
	TESCaravanCard();
	~TESCaravanCard();
};

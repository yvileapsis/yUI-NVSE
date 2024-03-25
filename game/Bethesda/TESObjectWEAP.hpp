#pragma once
#include "TESBoundObject.hpp"
#include "TESModelTextureSwap.hpp"
#include "TESIcon.hpp"
#include "TESScriptableForm.hpp"
#include "TESEnchantableForm.hpp"
#include "TESValueForm.hpp"
#include "TESWeightForm.hpp"
#include "TESHealthForm.hpp"
#include "TESAttackDamageForm.hpp"
#include "BGSAmmoForm.hpp"
#include "BGSClipRoundsForm.hpp"
#include "BGSDestructibleObjectForm.hpp"
#include "BGSRepairItemList.hpp"
#include "BGSEquipType.hpp"
#include "BGSPreloadable.hpp"
#include "BGSMessageIcon.hpp"
#include "BGSBipedModelList.hpp"
#include "BGSPickupPutdownSounds.hpp"

class BGSProjectile;
class SpellItem;
class BGSImpactDataSet;
class TESObjectSTAT;
class TESObjectIMOD;

// 0x388
class TESObjectWEAP :
	public TESBoundObject,				// 000
	public TESFullName,					// 030
	public TESModelTextureSwap,			// 03C
	public TESIcon,						// 05C
	public TESScriptableForm,			// 068
	public TESEnchantableForm,			// 074
	public TESValueForm,				// 084
	public TESWeightForm,				// 08C
	public TESHealthForm,				// 094
	public TESAttackDamageForm,			// 09C
	public BGSAmmoForm,					// 0A4
	public BGSClipRoundsForm,			// 0AC
	public BGSDestructibleObjectForm,	// 0B4
	public BGSRepairItemList,			// 0BC
	public BGSEquipType,				// 0C4
	public BGSPreloadable,				// 0CC
	public BGSMessageIcon,				// 0D0
	public BGSBipedModelList,			// 0E0
	public BGSPickupPutdownSounds		// 0E8
{
public:
	TESObjectWEAP();
	~TESObjectWEAP();

	enum EnumWeaponType {
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

	enum EnumWeaponSounds {
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

	enum EnumHandGrip {
		eHandGrip_Default = 0xFF,
		eHandGrip_1 = 0xE6,
		eHandGrip_2 = 0xE7,
		eHandGrip_3 = 0xE8,
		eHandGrip_4 = 0xE9,
		eHandGrip_5 = 0xEA,
		eHandGrip_6 = 0xEB,
		eHandGrip_Count = 7,
	};

	enum EnumAttackAnimations {
		eAttackAnim_Default = 255,
		eAttackAnim_Attack3 = 38,
		eAttackAnim_Attack4 = 44,
		eAttackAnim_Attack5 = 50,
		eAttackAnim_Attack6 = 56,
		eAttackAnim_Attack7 = 62,
		eAttackAnim_Attack8 = 68,
		eAttackAnim_Attack9 = 144,
		eAttackAnim_AttackLeft = 26,
		eAttackAnim_AttackLoop = 74,
		eAttackAnim_AttackRight = 32,
		eAttackAnim_AttackSpin = 80,
		eAttackAnim_AttackSpin2 = 86,
		eAttackAnim_AttackThrow = 114,
		eAttackAnim_AttackThrow2 = 120,
		eAttackAnim_AttackThrow3 = 126,
		eAttackAnim_AttackThrow4 = 132,
		eAttackAnim_AttackThrow5 = 138,
		eAttackAnim_AttackThrow6 = 150,
		eAttackAnim_AttackThrow7 = 156,
		eAttackAnim_AttackThrow8 = 162,
		eAttackAnim_PlaceMine = 102,
		eAttackAnim_PlaceMine2 = 108,
		eAttackAnim_Count = 23,
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

	enum WeaponFlags1 : UInt8 
	{
		kIgnoresNormalWeapResist	= 1 << 0,
		kIsAutomatic				= 1 << 1,
		kHasScope					= 1 << 2,
		kCantDrop					= 1 << 3,
		kHideBackpack				= 1 << 4,
		kEmbeddedWeapon				= 1 << 5,
		kNo1stPersonISAnims			= 1 << 6,
		kNonPlayable				= 1 << 7
	};

	enum WeaponFlags2 : UInt32 {
		kPlayerOnly					= 1 << 0,
		kNPCsUseAmmo				= 1 << 1,
		kNoJamAfterReload			= 1 << 2,
		kActionPointOverride		= 1 << 3,
		kMinorCrime					= 1 << 4,
		kFixedRange					= 1 << 5,
		kNotUsedNormalCombat		= 1 << 6,
		kDamageToWeaponOverride		= 1 << 7,
		kNo3rdPersonISAnims			= 1 << 8,
		kBurstShot					= 1 << 9,
		kRumbleAlternate			= 1 << 10,
		kLongBurst					= 1 << 11,
		kNightVision				= 1 << 12,
		kScopeFromMod				= 1 << 13
	};

	enum EnumEmbedWeapAV {
		eEmbedAV_Perception = 0,
		eEmbedAV_Endurance,
		eEmbedAV_LeftAttack,
		eEmbedAV_RightAttack,
		eEmbedAV_LeftMobility,
		eEmbedAV_RightMobility,
		eEmbedAV_Brain,
	};

	enum EnumOnHit {
		eOnHit_Normal = 0,
		eOnHit_DismemberOnly,
		eOnHit_ExplodeOnly,
		eOnHit_NoDismemberOrExplode,
	};

	enum EnumRumblePattern {
		eRumblePattern_Constant = 0,
		eRumblePattern_Square,
		eRumblePattern_Triangle,
		eRumblePattern_Sawtooth
	};

	enum EnumCritDamageFlags {
		eCritDamage_OnDeath = 0x1
	};

	enum WeaponReloadModes
	{
		kReload_Normal = 0x0,
		kReload_WeaponMod = 0x1,
		kReload_Looping = 0x2,
	};

	enum WeaponModEffects
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

	UInt8				eWeaponType;		// 0F4
	UInt8				pad[3];
	float				animMult;			// 0F8
	float				reach;				// 0FC
	WeaponFlags1		weaponFlags1;		// 100
	UInt8				handGrip;			// 101
	UInt8				ammoUse;			// 102
	UInt8				reloadAnim;			// 103
	float				minSpread;			// 104
	float				spread;				// 108
	UInt32				unk10C;				// 10C
	float				sightFOV;			// 110
	UInt32				unk114;				// 114
	BGSProjectile*		projectile;		// 118
	UInt8				baseVATSChance;		// 11C
	UInt8				attackAnim;			// 11D
	UInt8				numProjectiles;		// 11E
	UInt8				embedWeaponAV;		// 11F
	float				minRange;			// 120
	float				maxRange;			// 124
	UInt32				onHit;				// 128
	WeaponFlags2		weaponFlags2;		// 12C
	float				animAttackMult;		// 130
	float				fireRate;			// 134
	float				AP;					// 138
	float				rumbleLeftMotor;	// 13C
	float				rumbleRightMotor;	// 140
	float				rumbleDuration;		// 144
	float				damageToWeaponMult;	// 148
	float				animShotsPerSec;	// 14C
	float				animReloadTime;		// 150
	float				animJamTime;		// 154
	float				aimArc;				// 158
	UInt32				weaponSkill;		// 15C - actor value
	UInt32				rumblePattern;		// 160 - reload anim?
	float				rumbleWavelength;	// 164
	float				limbDamageMult;		// 168
	UInt32				resistType;			// 16c - actor value
	float				sightUsage;			// 170
	float				semiAutoFireDelayMin;	// 174
	float				semiAutoFireDelayMax;	// 178
	UInt32				unk17C;				// 17C - 0-0x10: 0x8:str req 0x10: - skill req  - 0xb:kill impulse B158 - mod 1 val B15C - Mod 2 val Effects: 0x1: e(zoom) 0x2: a 0x3:0 0x4-6: Values c-e Mod Effects Val2:1-3
	UInt32				effectMods[3];		// 180
	float				value1Mod[3];		// 18C
	UInt32				powerAttackAnimOverride;	// 198
	UInt32				strRequired;		// 19C
	UInt8				unk1A0;				// 1A0
	UInt8				modReloadAnim;		// 1A1
	UInt8				pad1A2[2];			// 1A2
	float				regenRate;			// 1A4
	float				killImpulse;		// 1A8
	float				value2Mod[3];		// 1AC
	float				impulseDist;		// 1B8
	UInt32				skillRequirement;	// 1BC
	UInt16				criticalDamage;		// 1C0
	UInt8				unk1C2[2];			// 1C2
	float				criticalPercent;	// 1C4
	UInt8				critDamageFlags;	// 1C8
	UInt8				pad1C9[3];			// 1C9
	SpellItem*			criticalEffect;	// 1CC
	TESModel			shellCasingModel;	// 1DO
	TESModel			targetNIF;			// 1E8 - target NIF
	TESModel			model200;			// 200 - could be a texture swap
	UInt32				unk218;				// 218
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
	BGSImpactDataSet*	impactDataSet;	// 24C
	TESObjectSTAT*		worldStatic;		// 250
	TESObjectSTAT*		mod1Static;		// 254
	TESObjectSTAT*		mod2Static;		// 258
	TESObjectSTAT*		mod3Static;		// 25C
	TESObjectSTAT*		mod12Static;		// 260
	TESObjectSTAT*		mod13Static;		// 264
	TESObjectSTAT*		mod23Static;		// 268
	TESObjectSTAT*		mod123Static;		// 26C
	TESModelTextureSwap	textureMod1;		// 270 Mod 1
	TESModelTextureSwap	textureMod2;		// 290 Mod 2
	TESModelTextureSwap	textureMod3;		// 2B0 Mod 3
	TESModelTextureSwap	textureMod12;		// 2D0 Mod 1-2
	TESModelTextureSwap	textureMod13;		// 2F0 Model 1-3
	TESModelTextureSwap	textureMod23;		// 310 Model 2-3
	TESModelTextureSwap	textureMod123;		// 330 Model 1-2-3
	TESObjectIMOD*		itemMod1;			// 350
	TESObjectIMOD*		itemMod2;			// 354
	TESObjectIMOD*		itemMod3;			// 358
	UInt32				unk35C;				// 35C
	UInt32				unk360;				// 360
	UInt32				soundLevel;			// 364
	UInt32				unk368;				// 368
	UInt32				unk36C;				// 36C
	UInt32				unk370;				// 370
	UInt32				unk374;				// 374
	UInt32				unk378;				// 378
	UInt32				unk37C;				// 37C
	UInt32				recharge;			// 380 maybe recharge
	UInt8				unk384;				// 384
	UInt8				unk385[3];			// 385

	bool					IsAutomatic() const { return weaponFlags1 & kIsAutomatic; }
	void					SetIsAutomatic(bool abAuto) { weaponFlags1 = (WeaponFlags1)(abAuto ? weaponFlags1 | kIsAutomatic : weaponFlags1 & ~kIsAutomatic); }
	bool					HasScope() const { return weaponFlags1 & kHasScope; }
	bool					IsPlayable() { return !IsNonPlayable(); }
	bool					IsNonPlayable() { return weaponFlags1 & kNonPlayable; }
	void					SetNonPlayable(bool abAuto) { weaponFlags1 = (WeaponFlags1)(abAuto ? weaponFlags1 | kNonPlayable : weaponFlags1 & ~kNonPlayable); }
	UInt8					HandGrip() const;
	void					SetHandGrip(UInt8 handGrip);
	UInt8					AttackAnimation() const;
	void					SetAttackAnimation(UInt8 attackAnim);
	TESObjectIMOD*			GetItemMod(UInt8 which);
	__forceinline UInt32	GetItemModEffect(UInt8 which)	{ which -= 1; assert(which < 3); return effectMods[which]; }
	__forceinline Float32	GetItemModValue1(UInt8 which)	{ which -= 1; assert(which < 3); return value1Mod[which]; }
	__forceinline Float32	GetItemModValue2(UInt8 which)	{ which -= 1; assert(which < 3); return value2Mod[which]; }
	Float32					GetEffectModValue(WeaponModEffects value, UInt8 second = 0);
	void					EjectShellCasing(TESObjectREFR* apReference);

	bool					HasNightVision() const { return weaponFlags2 & kNightVision; }
	bool					IsMeleeWeapon();
	bool					IsRangedWeapon();
	void					SetAttackAnimation(UInt32 attackAnim);
	TESAmmo*				GetAmmo();
	__forceinline int		GetClipRounds(bool hasExtendedClipMod) { return ThisCall<int>(0x4FE160, this, hasExtendedClipMod); };
	bool					CanEquipAmmo(TESAmmo* ammo);
	bool					IsThrownWeapon() { return eWeaponType >= kWeapType_OneHandGrenade; };
	BGSListForm*			GetAmmoList();
	__forceinline bool		IgnoresDTDR() const { return weaponFlags1 & kIgnoresNormalWeapResist; }

	float					GetWeaponValue(UInt32 whichVal);
	__forceinline bool		HasScopeAlt() const { return weaponFlags1 & (kHasScope) && !(weaponFlags2 & kScopeFromMod); }
	__forceinline TESAmmo*	GetEquippedAmmo(Actor* actor) { return ThisCall<TESAmmo*>(0x525980, this, actor); }

	bool IsMeleeWeapon() const;
	float GetModBonuses(UInt32 effectID);
};
static_assert(sizeof(TESObjectWEAP) == 0x388);
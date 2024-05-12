#pragma once

#include "TESForm.hpp"
#include "TESPackageData.hpp"

class BGSIdleCollection;
class TESCombatStyle;
class TESObjectREFR;
class TESIdleForm;
class TESTopic;
class Actor;

union PackageObject {
	TESForm*		pForm;
	TESObjectREFR* pReference;
	UInt32			uiObjectCode;
};

struct PackageSchedule {
	enum
	{
		kDay_Any = 0,
		kTime_Any = 0xFF,
	};

	enum
	{
		kMonth_January = 0,
		kMonth_February,
		kMonth_March,
		kMonth_April,
		kMonth_May,
		kMonth_June,
		kMonth_July,
		kMonth_August,
		kMonth_September,
		kMonth_October,
		kMonth_November,
		kMonth_December,
		kMonth_Spring,	// march, april, may
		kMonth_Summer,	// june, july, august
		kMonth_Autumn,	// september, august, november (in Geck)
		kMonth_Winter,	// december, january, february

		kMonth_Any = 0xFF,
	};

	enum
	{
		kWeekday_Sundays = 0,
		kWeekday_Morndays,
		kWeekday_Tuesdays,
		kWeekday_Wednesdays,
		kWeekday_Thursdays,
		kWeekday_Frydays,
		kWeekday_Saturdays,
		kWeekday_Weekdays,
		kWeekday_Weekends,
		kWeekday_MWF,
		kWeekday_TT,

		kWeekday_Any = 0xFF
	};

	SInt8	eMonth;
	SInt8	eDayOfWeek;
	SInt8	cDate;
	SInt8	cHour;
	SInt32	iDuration;

	static const char* MonthForCode(UInt8 monthCode);
	static const char* DayForCode(UInt8 dayCode);
	static UInt8 CodeForMonth(const char* monthStr);
	static UInt8 CodeForDay(const char* dayStr);
	static bool IsValidMonth(UInt8 m) { return (m + 1) <= kMonth_Winter; }
	static bool IsValidTime(UInt8 t) { return (t + 1) <= 24; }
	static bool IsValidDay(UInt8 d) { return (d + 1) <= kWeekday_TT; }
	static bool IsValidDate(UInt8 d) { return d <= 31; }
};

ASSERT_SIZE(PackageSchedule, 0x8);

struct PackageLocation {
	enum {
		kPackLocation_NearReference = 0,
		kPackLocation_InCell = 1,
		kPackLocation_CurrentLocation = 2,
		kPackLocation_EditorLocation = 3,
		kPackLocation_ObjectID = 4,
		kPackLocation_ObjectType = 5,
		kPackLocation_LinkedReference = 6,

		kPackLocation_Max,
	};

	UInt8			ucLocationType;
	UInt32			uiRadius;
	PackageObject	uObject;

	static PackageLocation* Create();
	static const char* StringForLocationCode(UInt8 locCode);
	const char* StringForLocationCodeAndData(void);
	static UInt8 LocationCodeForString(const char* locStr);
	static bool IsValidLocationType(UInt8 locCode) { return locCode < kPackLocation_Max; }
};

ASSERT_SIZE(PackageLocation, 0xC);

struct PackageTarget {
	UInt8			ucTargetType;
	PackageObject	uObject;
	SInt32			iValue;
	float			unk0C;

	static PackageTarget* Create();
	static const char* StringForTargetCode(UInt8 targetCode);
	const char* StringForTargetCodeAndData(void);
	static UInt8 TargetCodeForString(const char* targetStr);
	static bool IsValidTargetCode(UInt8 c) { return c < 3; }
};

ASSERT_SIZE(PackageTarget, 0x10);

struct PackageEventAction {
	TESIdleForm*	pIdle;
	Script*			pScript;
	TESTopic*		pTopic;
	UInt32			eType;
};

ASSERT_SIZE(PackageEventAction, 0x10);

class TESPackage : public TESForm {
public:
	TESPackage();
	~TESPackage();

	virtual bool	Unk_4E(void*);
	virtual bool	IsActorAtLocation(Actor* apActor, bool abIgnoreDistance, float afExtraRadius, bool abInFurniture);
	virtual bool	IsActorAtSecondLocation(Actor* apMobileObject, Actor* apPackageowner, bool abIgnoreDistance, float afExtraRadius, bool abInFurniture);
	virtual bool	IsActorAtRefTarget(Actor* apActor, SInt32 aiExtraRadius);
	virtual bool	IsTargetAtLocation(Actor* apActor, SInt32 aiExtraRadius);
	virtual UInt16	GetSaveSize();
	virtual UInt16	SaveGame();
	virtual UInt16	LoadGame();
	virtual void	Unk_56();
	virtual bool	IsPackageOwner(Actor* apActor = nullptr);

	enum	// From OBSE and FNVEdit
	{
		kPackageFlag_OffersServices = 1 << 0,
		kPackageFlag_MustReachLocation = 1 << 1,
		kPackageFlag_MustComplete = 1 << 2,
		kPackageFlag_LockDoorsAtStart = 1 << 3,
		kPackageFlag_LockDoorsAtEnd = 1 << 4, 	// set by CHANGE_PACKAGE_WAITING ?
		kPackageFlag_LockDoorsAtLocation = 1 << 5,
		kPackageFlag_UnlockDoorsAtStart = 1 << 6,
		kPackageFlag_UnlockDoorsAtEnd = 1 << 7,
		kPackageFlag_UnlockDoorsAtLocation = 1 << 8,
		kPackageFlag_ContinueIfPCNear = 1 << 9,
		kPackageFlag_OncePerDay = 1 << 10,
		kPackageFlag_Unk11 = 1 << 11,
		kPackageFlag_SkipFalloutBehavior = 1 << 12,
		kPackageFlag_AlwaysRun = 1 << 13,
		kPackageFlag_Unk14 = 1 << 14,
		kPackageFlag_NeverRun = 1 << 15,	// Save only ?
		kPackageFlag_Unk16 = 1 << 16,
		kPackageFlag_AlwaysSneak = 1 << 17,
		kPackageFlag_AllowSwimming = 1 << 18,
		kPackageFlag_AllowFalls = 1 << 19,
		kPackageFlag_ArmorUnequipped = 1 << 20,
		kPackageFlag_WeaponsUnequipped = 1 << 21,
		kPackageFlag_DefensiveCombat = 1 << 22,
		kPackageFlag_WeaponsDrawn = 1 << 23,
		kPackageFlag_NoIdleAnims = 1 << 24,
		kPackageFlag_PretendInCombat = 1 << 25,
		kPackageFlag_ContinueDuringCombat = 1 << 26,
		kPackageFlag_NoCombatAlert = 1 << 27,
		kPackageFlag_NoWarnAttackBehavior = 1 << 28,
		kPackageFlag_Unk29 = 1 << 29,
		kPackageFlag_Unk30 = 1 << 30,
		kPackageFlag_Unk31 = 1 << 31
	};

	enum Type {
		EXPLORE = 0,		// 00
		FOLLOW,
		ESCORT,
		EAT,
		SLEEP,
		WANDER,
		TRAVEL,
		ACCOMPANY,
		USE_ITEM_AT,
		AMBUSH,
		FLEE_NON_COMBAT,
		CAST_MAGIC,
		SANDBOX,
		PATROL,
		GUARD,
		DIALOGUE,
		USE_WEAPON,
		FIND				= 17,
		COMBAT				= 18,
		ALARM				= 21,
		FLEE				= 22,
		PTYPE_TRESPASS		= 23,
		SPECTATOR			= 24,
		IN_GAME_DIALOGUE	= 28,
		BACKUP				= 40,

		COUNT,
	};

	enum	// From OBSE and FNVEdit
	{
		kObjectType_None = 0,
		kObjectType_Activators,
		kObjectType_Armor,
		kObjectType_Books,
		kObjectType_Clothing,
		kObjectType_Containers,
		kObjectType_Doors,
		kObjectType_Ingredients,
		kObjectType_Lights,
		kObjectType_Misc,
		kObjectType_Flora,
		kObjectType_Furniture,
		kObjectType_WeaponsAny,
		kObjectType_Ammo,
		kObjectType_NPCs,
		kObjectType_Creatures,
		kObjectType_Keys,				//	10
		kObjectType_Alchemy,
		kObjectType_Food,
		kObjectType_AllCombatWearable,
		kObjectType_AllWearable,
		kObjectType_WeaponsRanged,
		kObjectType_WeaponsMelee,
		kObjectType_WeaponsNone,
		kObjectType_ActorEffectAny,
		kObjectType_ActorEffectRangeTarget,
		kObjectType_ActorEffectRangeTouch,
		kObjectType_ActorEffectRangeSelf,
		kObjectType_ActorsAny,

		kObjectType_Max,						//	1E
	};

	enum {
		kTargetType_Refr = 0,
		kTargetType_BaseObject = 1,
		kTargetType_TypeCode = 2,

		kTargetType_Max = 3,
	};

	enum eProcedure {			// UInt32	// Checked the Geck Wiki. Not consistent with s_procNames (which has a diffferent order and 0x37 procedures)
		kProcedure_TRAVEL = 0,
		kProcedure_ACTIVATE,
		kProcedure_ACQUIRE,
		kProcedure_WAIT,
		kProcedure_DIALOGUE,
		kProcedure_GREET,
		kProcedure_GREET_DEAD,
		kProcedure_WANDER,
		kProcedure_SLEEP,
		kProcedure_OBSERVE_COMBAT,
		kProcedure_EAT,
		kProcedure_FOLLOW,
		kProcedure_ESCORT,
		kProcedure_COMBAT,
		kProcedure_ALARM,
		kProcedure_PURSUE,
		kProcedure_FLEE,					// 0x10
		kProcedure_DONE,
		kProcedure_YELD,
		kProcedure_TRAVEL_TARGET,
		kProcedure_CREATE_FOLLOW,
		kProcedure_GET_UP,
		kProcedure_MOUNT_HORSE,
		kProcedure_DISMOUNT_HORSE,
		kProcedure_DO_NOTHING,
		kProcedure_UNK019,
		kProcedure_UNK01A,
		kProcedure_ACCOMPANY,
		kProcedure_USE_ITEM_AT,
		kProcedure_SANDMAN,
		kProcedure_WAIT_AMBUSH,
		kProcedure_SURFACE,					// 0x20
		kProcedure_WAIT_FOR_SPELL,
		kProcedure_CHOOSE_CAST,
		kProcedure_FLEE_NON_COMBAT,
		kProcedure_REMOVE_WORN_ITEMS,
		kProcedure_SEARCH,
		kProcedure_CLEAR_MOUNT_POSITION,
		kProcedure_SUMMON_CREATURE_DEFEND,
		kProcedure_AVOID_AREA,
		kProcedure_UNEQUIP_ARMOR,
		kProcedure_PATROL,
		kProcedure_USE_WEAPON,
		kProcedure_DIALOGUE_ACTIVATE,
		kProcedure_GUARD,
		kProcedure_SANDBOX,
		kProcedure_USE_IDLE_MARKER,
		kProcedure_TAKE_BACK_ITEM,
		kProcedure_SITTING,					// 0x30
		kProcedure_MOVEMENT_BLOCKED,
		kProcedure_CANIBAL_FEED,			// 0x32

		kProcedure_MAX						// 0x33
	};

	struct Data {
		UInt32	eProcedureType;
		UInt32	uiPackageFlags;
		UInt8	ucType;
		UInt16	usBehaviorFlags;
		UInt16	usSpecificFlags;
	};

	Data				kData;
	TESPackageData*		pPackageData;
	PackageLocation*	pPackageLocation;
	PackageTarget*		pPackageTarget;
	BGSIdleCollection*	pIdleCollection;
	PackageSchedule		kPackSched;
	UInt32				unk040;
	UInt32				unk044;
	TESCombatStyle*		pCombatStyle;
	PackageEventAction	kOnBegin;
	PackageEventAction	kOnEnd;
	PackageEventAction	kOnChange;
	UInt32				unk07C;

#if 0
	void SetTarget(TESObjectREFR* refr);
	void SetTarget(TESForm* baseForm, UInt32 count);
	void SetTarget(UInt8 typeCode, UInt32 count);
	void SetCount(UInt32 aCount);
	void SetDistance(UInt32 aDistance) { SetCount(aDistance); }
	PackageTarget* GetTargetData();
	PackageLocation* GetLocationData();

	bool IsFlagSet(UInt32 flag);
	void SetFlag(UInt32 flag, bool bSet);

	static const char* StringForPackageType(UInt32 pkgType);
	static const char* StringForObjectCode(UInt8 objCode);
	static UInt8 ObjectCodeForString(const char* objString);
	static bool IsValidObjectCode(UInt8 o) { return o < kObjectType_Max; }
	static const char* StringForProcedureCode(eProcedure proc);
#endif
};

ASSERT_SIZE(TESPackage, 0x80);
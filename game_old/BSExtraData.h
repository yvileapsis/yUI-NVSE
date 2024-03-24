#pragma once
#include <Containers.h>
#include <Utilities.hpp>
#include <Sound.h>
#include <InventoryChanges.h>

/*    Class							     vtbl	  Type  Size
 *   ----------------------------		------		--  --
 *	ExtraAction                        ????????		0E	14
 *	ExtraActivateLoopSound             ????????		87	18
 *	ExtraActivateRef                   ????????		53	18
 *	ExtraActivateRefChildren           ????????		54	18
 *	ExtraAmmo                          ????????		6E	14
 *	ExtraAnim                          ????????		10	10
 *	ExtraAshPileRef                    ????????		89	10
 *	ExtraCannotWear                    ????????		3E	0C	// no data
 *	ExtraCell3D                        ????????		2	10
 *	ExtraCellAcousticSpace             ????????		81	10
 *	ExtraCellCanopyShadowMask          ????????		0A	1C
 *	ExtraCellClimate                   ????????		8	10
 *	ExtraCellImageSpace                ????????		59	10
 *	ExtraCellMusicType                 ????????		7	10
 *	ExtraCellWaterType                 ????????		3	10
 *	ExtraCharge                        ????????		28	10
 *	ExtraCollisionData                 ????????		72	10
 *	ExtraCombatStyle                   ????????		69	10
 *	ExtraContainerChanges              ????????		15	10
 *	ExtraCount                         ????????		24	10
 *	ExtraCreatureAwakeSound            ????????		7D	18
 *	ExtraCreatureMovementSound         ????????		8A	18
 *	ExtraDecalRefs                     ????????		57	14
 *	ExtraDetachTime                    ????????		0B	10
 *	ExtraDismemberedLimbs              ????????		5F	30
 *	ExtraDistantData                   ????????		13	18
 *	ExtraDroppedItemList               ????????		3A	14
 *	ExtraEditorRefMovedData            ????????		4C	30
 *	ExtraEmittanceSource               ????????		67	10
 *	ExtraEnableStateChildren           ????????		38	14
 *	ExtraEnableStateParent             ????????		37	14
 *	ExtraEncounterZone                 ????????		74	10
 *	ExtraFactionChanges                ????????		5E	10
 *	ExtraFollower                      ????????		1D	10
 *	ExtraFollowerSwimBreadcrumbs       ????????		8B	28
 *	ExtraFriendHits                    ????????		45	1C
 *	ExtraGhost                         ????????		1F	0C	// no data
 *	ExtraGlobal                        ????????		22	10
 *	ExtraGuardedRefData                ????????		7C	1C
 *	ExtraHasNoRumors                   ????????		4E	10
 *	ExtraHavok                         ????????		1	14
 *	ExtraHeadingTarget                 ????????		46	10
 *	ExtraHealth                        ????????		25	10
 *	ExtraHealthPerc                    ????????		7A	10
 *	ExtraHotkey                        ????????		4A	10
 *	ExtraIgnoredBySandbox              ????????		80	0C	// no data
 *	ExtraInfoGeneralTopic              ????????		4D	10
 *	ExtraItemDropper                   ????????		39	10
 *	ExtraLastFinishedSequence          ????????		41	10
 *	ExtraLevCreaModifier               ????????		1E	10
 *	ExtraLeveledCreature               ????????		2E	14
 *	ExtraLeveledItem                   ????????		2F	14
 *	ExtraLight                         ????????		29	10
 *	ExtraLinkedRef                     ????????		51	10
 *	ExtraLinkedRefChildren             ????????		52	14
 *	ExtraLitWaterRefs                  ????????		85	14
 *	ExtraLock                          ????????		2A	10
 *	ExtraMapMarker                     ????????		2C	10
 *	ExtraMerchantContainer             ????????		3C	10
 *	ExtraModelSwap                     ????????		5B	14
 *	ExtraMultiBound                    ????????		61	10
 *	ExtraMultiBoundData                ????????		62	10
 *	ExtraMultiBoundRef                 ????????		63	10
 *	ExtraNavMeshPortal                 ????????		5A	14
 *	ExtraNorthRotation                 ????????		43	10
 *	ExtraObjectHealth                  ????????		56	10
 *	ExtraOcclusionPlane                ????????		71	10
 *	ExtraOcclusionPlaneRefData         ????????		76	10
 *	ExtraOpenCloseActivateRef          ????????		6C	10
 *	ExtraOriginalReference             ????????		20	10
 *	ExtraOwnership                     ????????		21	10
 *	ExtraPackage                       ????????		19	1C
 *	ExtraPackageData                   ????????		70	10
 *	ExtraPackageStartLocation          ????????		18	1C
 *	ExtraPatrolRefData                 ????????		6F	10
 *	ExtraPatrolRefInUseData            ????????		88	10
 *	ExtraPersistentCell                ????????		0C	10
 *	ExtraPlayerCrimeList               ????????		35	10
 *	ExtraPoison                        ????????		3F	10
 *	ExtraPortal                        ????????		78	10
 *	ExtraPortalRefData                 ????????		77	10
 *	ExtraPrimitive                     ????????		6B	10
 *	ExtraProcessMiddleLow              ????????		9	10
 *	ExtraRadiation                     ????????		5D	10
 *	ExtraRadioData                     ????????		68	1C
 *	ExtraRadius                        ????????		5C	10
 *	ExtraRagdollData                   ????????		14	10
 *	ExtraRandomTeleportMarker          ????????		3B	10
 *	ExtraRank                          ????????		23	10
 *	ExtraReferencePointer              ????????		1C	10
 *	ExtraReflectedRefs                 ????????		65	14
 *	ExtraReflectorRefs                 ????????		66	14
 *	ExtraRefractionProperty            ????????		48	10
 *	ExtraRegionList                    ????????		4	10
 *	ExtraReservedMarkers               ????????		82	10
 *	ExtraRoom                          ????????		79	10
 *	ExtraRoomRefData                   ????????		7B	10
 *	ExtraRunOncePacks                  ????????		1B	10
 *	ExtraSavedAnimation                ????????		42	10
 *	ExtraSavedHavokData                ????????		3D	10
 *	ExtraSayToTopicInfo                ????????		75	18
 *	ExtraSayTopicInfoOnceADay          ????????		73	10
 *	ExtraScale                         ????????		30	10
 *	ExtraScript                        ????????		0D	14
 *	ExtraSeed                          ????????		31	10
 *	ExtraSeenData                      ????????		5	24	: ExtraIntSeenData is also 5 but 2C
 *	ExtraSound                         ????????		4F	18
 *	ExtraStartingPosition              ????????		0F	24
 *	ExtraStartingWorldOrCell           ????????		49	10
 *	ExtraTalkingActor                  ????????		55	10
 *	ExtraTeleport                      ????????		2B	10
 *	ExtraTerminalState                 ????????		50	10
 *	ExtraTimeLeft                      ????????		27	10
 *	ExtraTrespassPackage               ????????		1A	10
 *	ExtraUsedMarkers                   ????????		12	10
 *	ExtraUses                          ????????		26	10
 *	ExtraWaterLightRefs                ????????		84	14
 *	ExtraWaterZoneMap                  ????????		7E	20
 *	ExtraWeaponAttackSound             ????????		86	18
 *	ExtraWeaponIdleSound               ????????		83	18
 *	ExtraWeaponModFlags                ????????		8D	10
 *	ExtraWorn                          ????????		16	0C	// no data
 *	ExtraWornLeft                      ????????		17	0C	// no data
 *	ExtraXTarget                       ????????		44	10
 */

 /* BaseExtraList methods:
	 AddExtra		0x0040A180
	 GetByType		0x0040A320, pass typeID
	 ItemsInList		0x0040A130
	 RemoveExtra		0x0040A250

   ExtraDataList methods:
	 DuplicateExtraListForContainer	0x0041B090
 */

enum {
	kExtraData_Havok					= 0x01,
	kExtraData_Cell3D					= 0x02,
	kExtraData_CellWaterType			= 0x03,
	kExtraData_RegionList				= 0x04,
	kExtraData_SeenData					= 0x05,
	//									= 0x06
	kExtraData_CellMusicType			= 0x07,
	kExtraData_CellClimate				= 0x08,
	kExtraData_ProcessMiddleLow			= 0x09,
	kExtraData_CellCanopyShadowMask		= 0x0A,
	kExtraData_DetachTime				= 0x0B,
	kExtraData_PersistentCell			= 0x0C,
	kExtraData_Script					= 0x0D,
	kExtraData_Action					= 0x0E,
	kExtraData_StartingPosition			= 0x0F,
	kExtraData_Anim						= 0x10,
	kExtraData_UsedMarkers				= 0x12,
	kExtraData_DistantData				= 0x13,
	kExtraData_RagdollData				= 0x14,
	kExtraData_ContainerChanges			= 0x15,
	kExtraData_Worn						= 0x16,
	kExtraData_WornLeft					= 0x17,
	kExtraData_PackageStartLocation		= 0x18,
	kExtraData_Package					= 0x19,
	kExtraData_TrespassPackage			= 0x1A,
	kExtraData_RunOncePacks				= 0x1B,
	kExtraData_ReferencePointer			= 0x1C,
	kExtraData_Follower					= 0x1D,
	kExtraData_LevCreaModifier			= 0x1E,
	kExtraData_Ghost					= 0x1F,
	kExtraData_OriginalReference		= 0x20,
	kExtraData_Ownership				= 0x21,
	kExtraData_Global					= 0x22,
	kExtraData_Rank						= 0x23,
	kExtraData_Count					= 0x24,
	kExtraData_Health					= 0x25,
	kExtraData_Uses						= 0x26,
	kExtraData_TimeLeft					= 0x27,
	kExtraData_Charge					= 0x28,
	kExtraData_Light					= 0x29,
	kExtraData_Lock						= 0x2A,
	kExtraData_Teleport					= 0x2B,
	kExtraData_MapMarker				= 0x2C,
	kExtraData_LeveledCreature			= 0x2E,
	kExtraData_LeveledItem				= 0x2F,
	kExtraData_Scale					= 0x30,
	kExtraData_Seed						= 0x31,
	kExtraData_NonActorMagicCaster		= 0x32,
	kExtraData_NonActorMagicTarget		= 0x33,
	kExtraData_PlayerCrimeList			= 0x35,
	kExtraData_EnableStateParent		= 0x37,
	kExtraData_EnableStateChildren		= 0x38,
	kExtraData_ItemDropper				= 0x39,
	kExtraData_DroppedItemList			= 0x3A,
	kExtraData_RandomTeleportMarker		= 0x3B,
	kExtraData_MerchantContainer		= 0x3C,
	kExtraData_SavedHavokData			= 0x3D,
	kExtraData_CannotWear				= 0x3E,
	kExtraData_Poison					= 0x3F,
	kExtraData_Unk040					= 0x40,	// referenced during LoadFormInModule (in oposition to kExtraData_Light)
	kExtraData_LastFinishedSequence		= 0x41,
	kExtraData_SavedAnimation			= 0x42,
	kExtraData_NorthRotation			= 0x43,
	kExtraData_XTarget					= 0x44,
	kExtraData_FriendHits				= 0x45,
	kExtraData_HeadingTarget			= 0x46,
	kExtraData_RefractionProperty		= 0x48,
	kExtraData_StartingWorldOrCell		= 0x49,
	kExtraData_Hotkey					= 0x4A,
	kExtraData_EditorRefMovedData		= 0x4C,
	kExtraData_InfoGeneralTopic			= 0x4D,
	kExtraData_HasNoRumors				= 0x4E,
	kExtraData_Sound					= 0x4F,
	kExtraData_TerminalState			= 0x50,
	kExtraData_LinkedRef				= 0x51,
	kExtraData_LinkedRefChildren		= 0x52,
	kExtraData_ActivateRef				= 0x53,
	kExtraData_ActivateRefChildren		= 0x54,
	kExtraData_TalkingActor				= 0x55,
	kExtraData_ObjectHealth				= 0x56,
	kExtraData_DecalRefs				= 0x57,
	//									= 0x58,
	kExtraData_CellImageSpace			= 0x59,
	kExtraData_NavMeshPortal			= 0x5A,
	kExtraData_ModelSwap				= 0x5B,
	kExtraData_Radius					= 0x5C,
	kExtraData_Radiation				= 0x5D,
	kExtraData_FactionChanges			= 0x5E,
	kExtraData_DismemberedLimbs			= 0x5F,
	kExtraData_MultiBound				= 0x61,
	kExtraData_MultiBoundData			= 0x62,
	kExtraData_MultiBoundRef			= 0x63,
	kExtraData_ReflectedRefs			= 0x65,
	kExtraData_ReflectorRefs			= 0x66,
	kExtraData_EmittanceSource			= 0x67,
	kExtraData_RadioData				= 0x68,
	kExtraData_CombatStyle				= 0x69,
	kExtraData_Primitive				= 0x6B,
	kExtraData_OpenCloseActivateRef		= 0x6C,
	kExtraData_AnimNoteReciever			= 0x6D,
	kExtraData_Ammo						= 0x6E,
	kExtraData_PatrolRefData			= 0x6F,
	kExtraData_PackageData				= 0x70,
	kExtraData_OcclusionPlane			= 0x71,
	kExtraData_CollisionData			= 0x72,
	kExtraData_SayTopicInfoOnceADay		= 0x73,
	kExtraData_EncounterZone			= 0x74,
	kExtraData_SayToTopicInfo			= 0x75,
	kExtraData_OcclusionPlaneRefData	= 0x76,
	kExtraData_PortalRefData			= 0x77,
	kExtraData_Portal					= 0x78,
	kExtraData_Room						= 0x79,
	kExtraData_HealthPerc				= 0x7A,
	kExtraData_RoomRefData				= 0x7B,
	kExtraData_GuardedRefData			= 0x7C,
	kExtraData_CreatureAwakeSound		= 0x7D,
	kExtraData_WaterZoneMap				= 0x7E,
	kExtraData_IgnoredBySandbox			= 0x80,
	kExtraData_CellAcousticSpace		= 0x81,
	kExtraData_ReservedMarkers			= 0x82,
	kExtraData_WeaponIdleSound			= 0x83,
	kExtraData_WaterLightRefs			= 0x84,
	kExtraData_LitWaterRefs				= 0x85,
	kExtraData_WeaponAttackSound		= 0x86,
	kExtraData_ActivateLoopSound		= 0x87,
	kExtraData_PatrolRefInUseData		= 0x88,
	kExtraData_AshPileRef				= 0x89,
	kExtraData_CreatureMovementSound	= 0x8A,
	kExtraData_FollowerSwimBreadcrumbs	= 0x8B,
	//									= 0x8C,
	kExtraData_WeaponModFlags			= 0x8D,
	//									= 0x8E
	//									= 0x8F
	kExtraData_0x90						= 0x90,	// referenced in LoadGame but no data
	kExtraData_0x91						= 0x91,	// referenced in LoadGame but no data
	kExtraData_SpecialRenderFlags		= 0x92
};
#define GetExtraType(xDataList, Type) (Extra ## Type*)xDataList.GetByType(kExtraData_ ## Type)

class BSExtraData
{
public:
	BSExtraData();
	virtual ~BSExtraData();

	virtual bool				Differs(BSExtraData* extra);	// 001
	static BSExtraData*			Create(UInt8 xType, UInt32 size, UInt32 vtbl);

//	void			** _vtbl;	// 000
	UInt8			type;		// 004
	UInt8			pad[3];		// 005
	BSExtraData*	next;		// 008
};
static_assert(sizeof(BSExtraData) == 0xC);


#define				GetByTypeCast(xDataList, Type) DYNAMIC_CAST(xDataList.GetByType(kExtraData_ ## Type), BSExtraData, Extra ## Type)
extern const char*	GetExtraDataName(UInt8 ExtraDataType);
SInt32				GetCountForExtraDataList(ExtraDataList* list);

class ExtraSound : public BSExtraData
{
public:
	ExtraSound();
	~ExtraSound();

	Sound	sound; // 0x0C
};
static_assert(sizeof(ExtraSound) == 0x18);

class ExtraDroppedItemList : public BSExtraData
{
public:
	TList<TESObjectREFR>	itemRefs;	// 0x0C
};
static_assert(sizeof(ExtraDroppedItemList) == 0x14);

class ExtraAction : public BSExtraData
{
public:
	ExtraAction();
	virtual ~ExtraAction();

	UInt8			flags0C;	// 00C	some kind of status or flags: 
	//		if not RunOnActivateBlock: bit0 and bit1 are set before TESObjectREF::Activate, bit0 is preserved, bit1 is cleared after returning.
	UInt8			fill0D[3];	// 00D
	TESForm* actionRef;	// 010

	static ExtraAction* Create();
};
static_assert(sizeof(ExtraAction) == 0x14);

class ExtraScript : public BSExtraData
{
public:
	ExtraScript();
	virtual ~ExtraScript();

	Script* script;		// 00C
	ScriptEventList* eventList;	// 010

	static ExtraScript*		Create(TESScriptableForm* baseForm = NULL, bool create = true, TESObjectREFR* container = NULL);
};
static_assert(sizeof(ExtraScript) == 0x14);

class ExtraContainerChanges : public BSExtraData
{
public:
	ExtraContainerChanges();
	virtual ~ExtraContainerChanges();

	// find the equipped item whose form matches the passed matcher

	ExtraContainerChangesData* data;	// 00C

	static ExtraContainerChanges*	Create();
	static ExtraContainerChanges*	Create(TESObjectREFR* thisObj, UInt32 refID = 0, UInt32 count = 1,
		ExtendDataList* pExtendDataList = NULL);
	ExtraContainerChanges*  GetForRef(TESObjectREFR* refr);

};
static_assert(sizeof(ExtraContainerChanges) == 0x10);

class ExtraHealth : public BSExtraData
{
public:
	ExtraHealth();
	virtual ~ExtraHealth();
	float health;

	static ExtraHealth* Create();
};
static_assert(sizeof(ExtraHealth) == 0x10);

class ExtraWorn : public BSExtraData	// Item is equipped
{
public:
	ExtraWorn();
	virtual ~ExtraWorn();

	static ExtraWorn* Create();
};
static_assert(sizeof(ExtraWorn) == 0x0C);

class ExtraWornLeft : public BSExtraData	// haven't seen used yet
{
public:
	ExtraWornLeft();
	virtual ~ExtraWornLeft();

	static ExtraWornLeft* Create();
};
static_assert(sizeof(ExtraWornLeft) == 0x0C);

class ExtraCannotWear : public BSExtraData	//	Seen used as ForceEquip ! Unused as forceUnequip (bug?)
{
public:
	ExtraCannotWear();
	virtual ~ExtraCannotWear();

	static ExtraCannotWear* Create();
};
static_assert(sizeof(ExtraCannotWear) == 0x0C);

class ExtraHotkey : public BSExtraData
{
public:
	ExtraHotkey();
	virtual ~ExtraHotkey();

	UInt8	index;		// 00C (is 0-7)

	static ExtraHotkey* Create();
};
static_assert(sizeof(ExtraHotkey) == 0x10);

class ExtraCount : public BSExtraData
{
public:
	ExtraCount();
	virtual ~ExtraCount();

	SInt16	count;	// 00C
	UInt8	pad[2];	// 00E

	static ExtraCount* Create(UInt32 count = 0);
};
static_assert(sizeof(ExtraCount) == 0x10);

class ExtraLock : public BSExtraData
{
public:
	ExtraLock();
	virtual ~ExtraLock();

	ExtraLockData* data;		// 00C

	static ExtraLock* Create();
};
static_assert(sizeof(ExtraLock) == 0x10);

class ExtraUses : public BSExtraData
{
public:
	ExtraUses();
	~ExtraUses();

	UInt32 unk0;

	static ExtraUses* Create();
};
static_assert(sizeof(ExtraUses) == 0x10);

class ExtraTeleport : public BSExtraData
{
public:
	ExtraTeleport();
	~ExtraTeleport();

	ExtraTeleportData* data;

	static ExtraTeleport* Create();
};
static_assert(sizeof(ExtraTeleport) == 0x10);

class ExtraRandomTeleportMarker : public BSExtraData
{
public:
	ExtraRandomTeleportMarker();
	~ExtraRandomTeleportMarker();

	TESObjectREFR* teleportRef;
};
static_assert(sizeof(ExtraRandomTeleportMarker) == 0x10);

class ExtraAmmo : public BSExtraData
{
public:
	ExtraAmmo();
	~ExtraAmmo();

	TESAmmo* ammo;	// 0xC
	UInt32		count;	// 0x10
};
static_assert(sizeof(ExtraAmmo) == 0x14);

class ExtraOwnership : public BSExtraData
{
public:
	ExtraOwnership();
	virtual ~ExtraOwnership();

	TESForm* owner;	// maybe this should be a union {TESFaction*; TESNPC*} but it would be more unwieldy to access and modify

	static ExtraOwnership* Create();
};
static_assert(sizeof(ExtraOwnership) == 0x10);

class ExtraRank : public BSExtraData
{
public:
	ExtraRank();
	virtual ~ExtraRank();

	UInt32	rank; // 00C

	static ExtraRank* Create();
};
static_assert(sizeof(ExtraRank) == 0x10);

class ExtraGlobal : public BSExtraData
{								//ownership data, stored separately from ExtraOwnership
public:
	ExtraGlobal();
	virtual ~ExtraGlobal();

	TESGlobal* globalVar;	// 00C
};
static_assert(sizeof(ExtraGlobal) == 0x10);

class ExtraWeaponModFlags : public BSExtraData
{
public:
	ExtraWeaponModFlags();
	~ExtraWeaponModFlags();

	UInt8	flags; // 00C

	static ExtraWeaponModFlags* Create();
};
static_assert(sizeof(ExtraWeaponModFlags) == 0x10);

class ExtraFactionChanges : public BSExtraData
{
public:
	ExtraFactionChanges();
	virtual ~ExtraFactionChanges();

	FactionListData* data;

	static ExtraFactionChanges* Create();
};
static_assert(sizeof(ExtraFactionChanges) == 0x10);

FactionListData* GetExtraFactionList(BaseExtraList& xDataList);
SInt8									GetExtraFactionRank(BaseExtraList& xDataList, TESFaction* faction);
void									SetExtraFactionRank(BaseExtraList& xDataList, TESFaction* faction, SInt8 rank);

class ExtraLeveledCreature : public BSExtraData
{
public:
	ExtraLeveledCreature();
	virtual ~ExtraLeveledCreature();

	TESActorBase* baseForm;		// 00C
	TESActorBase* actorBase;		// 010
};
static_assert(sizeof(ExtraLeveledCreature) == 0x14);

// PackageStartLocation = Worldspace or Cell / PosX / PosY / PosZ / and 4 bytes

class ExtraCombatStyle : public BSExtraData
{
public:
	ExtraCombatStyle();
	virtual ~ExtraCombatStyle();

	TESCombatStyle* combatStyle;	// 00C
};
static_assert(sizeof(ExtraCombatStyle) == 0x10);

class ExtraReferencePointer : public BSExtraData
{
public:
	ExtraReferencePointer();
	virtual ~ExtraReferencePointer();

	TESObjectREFR* refr;			// 00C
};
static_assert(sizeof(ExtraReferencePointer) == 0x10);

// Provided by "Luthien Anarion"
class ExtraMapMarker : BSExtraData
{
public:
	ExtraMapMarker();
	~ExtraMapMarker();

	ExtraMapMarkerData* data;
};
static_assert(sizeof(ExtraMapMarker) == 0x10);

class ExtraNorthRotation : public BSExtraData
{
public:
	ExtraNorthRotation();
	virtual ~ExtraNorthRotation();

	UInt32				angle;		// 00C
};
static_assert(sizeof(ExtraNorthRotation) == 0x10);

class ExtraSeenData : public BSExtraData
{
public:
	ExtraSeenData();
	virtual ~ExtraSeenData();

	UInt8 unk[0x24 - 0x0C];		// 00C
};
static_assert(sizeof(ExtraSeenData) == 0x24);

class ExtraIntSeenData : public ExtraSeenData
{
public:
	ExtraIntSeenData();
	virtual ~ExtraIntSeenData();

	UInt8				coordX;		// 024
	UInt8				coordY;		// 025
	UInt8				filler[2];	// 026
	ExtraIntSeenData* next;		// 028
};
static_assert(sizeof(ExtraIntSeenData) == 0x2C);

// ExtraUsedMarkers is a bitmask of 30 bits.

class ExtraPersistentCell : public BSExtraData
{
public:
	ExtraPersistentCell();
	virtual ~ExtraPersistentCell();

	TESObjectCELL* persistentCell;	// 0C
};
static_assert(sizeof(ExtraPersistentCell) == 0x10);

class ExtraTerminalState : public BSExtraData
{
public:
	ExtraTerminalState();
	virtual ~ExtraTerminalState();

	enum
	{
		kFlag_Locked = 1 << 7,        // terminal is locked
	};

	UInt8	flags;
	UInt8	lockLevel;
	UInt8	filler[2];
};
static_assert(sizeof(ExtraTerminalState) == 0x10);

// 10
class ExtraLinkedRef : public BSExtraData
{
public:
	TESObjectREFR* linkedRef;		// 0C

	static ExtraLinkedRef* __stdcall Create(TESObjectREFR* _linkedRef = NULL);
};

// 14
class ExtraLinkedRefChildren : public BSExtraData
{
public:
	TList<TESObjectREFR>	children;	// 0C
};

// 20
class ExtraActivateRef : public BSExtraData
{
public:
	struct parentRef
	{
		TESObjectREFR* ref;
		float			delay;
	};
	TList<parentRef>	parentRefs;
	UInt32				flags;
	String              activationPromptOverride;
};

// 10
class ExtraTalkingActor : public BSExtraData
{
public:
	Actor* actor;		// 0C
};

// 10
class ExtraObjectHealth : public BSExtraData
{
public:
	float			health;		// 0C

	static ExtraObjectHealth* __stdcall Create(float _health);
};


class ExtraAnim : public BSExtraData
{
public:
	ExtraAnim();
	virtual ~ExtraAnim();

	struct Animation
	{
	};	// 013C

	Animation* data;	// 0C
};
static_assert(sizeof(ExtraAnim) == 0x10);

class ExtraPoison : public BSExtraData
{
public:
	ExtraPoison();
	virtual ~ExtraPoison();

	AlchemyItem* poisonEffect;    // 0C
};
static_assert(sizeof(ExtraPoison) == 0x10);

class ExtraAshPileRef : public BSExtraData
{
public:
	ExtraAshPileRef();
	~ExtraAshPileRef();

	TESObjectREFR* sourceRef;		// 0C
};
static_assert(sizeof(ExtraAshPileRef) == 0x10);
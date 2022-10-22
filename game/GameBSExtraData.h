#pragma once
#include <Containers.h>
#include <Utilities.h>

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

struct BaseExtraList
{
	bool						HasType(UInt32 type) const;
	__forceinline BSExtraData*	GetByType(UInt8 type) const { return ThisStdCall<BSExtraData*>(0x410220, this, type); };
	void						MarkType(UInt32 type, bool bCleared);
	__forceinline void			Remove(BSExtraData* toRemove, bool doFree) { ThisStdCall(0x410020, this, toRemove, doFree); }
	__forceinline void			RemoveByType(UInt32 type) { ThisStdCall(0x410140, this, type); }
	__forceinline BSExtraData*	Add(BSExtraData* toAdd) { return ThisStdCall<BSExtraData*>(0x40FF60, this, toAdd); }
	__forceinline void			RemoveAll(bool doFree) { ThisStdCall(0x40FAE0, this, doFree); }
	bool						MarkScriptEvent(UInt32 eventMask, TESForm* eventTarget);
	__forceinline void			Copy(BaseExtraList* from) { ThisStdCall(0x411EC0, this, from); }
	void						DebugDump() const;
	bool						IsWorn();

	virtual	BaseExtraList*		Destroy(char chr);

	BSExtraData*	m_data;						// 004
	UInt8			m_presenceBitfield[0x15];	// 008 - if a bit is set, then the extralist should contain that extradata
	UInt8			pad1D[3];					// 01D
};
static_assert(sizeof(BaseExtraList) == 0x020);

struct ExtraDataList : public BaseExtraList
{
	static ExtraDataList * Create(BSExtraData* xBSData = NULL);
};
static_assert(sizeof(ExtraDataList) == 0x020);

class ExtendDataList : public TList<ExtraDataList>
{
public:
	static ExtendDataList* Create(ExtraDataList* pExtraDataList = nullptr);
	static void Free(ExtendDataList* xData, bool bFreeList = false);
	static bool Add(ExtendDataList* xData, ExtraDataList* xList);
	static bool Remove(ExtendDataList* xData, ExtraDataList* xList, bool bFreeList = false);

	SInt32 AddAt(ExtraDataList* item, SInt32 index);
	void RemoveAll() const;
	ExtraDataList* RemoveNth(SInt32 n);
};

typedef std::vector<ExtendDataList*> ExtendDataArray;

struct InventoryChanges
{
	ExtendDataList*			extendData;		// 00
	SInt32					countDelta;		// 04
	union
	{
		TESForm*			form;			// 08
		TESObjectWEAP*		weapon;			// 08
		TESAmmo*			ammo;			// 08
	};

	void					Free(bool bFreeList = false);
	void					Cleanup();
	static InventoryChanges*Create(TESForm* pForm, UInt32 count = 1, ExtendDataList* pExtendDataList = nullptr);
	ExtendDataList*			Add(ExtraDataList* newList);
	bool					Remove(ExtraDataList* toRemove, bool bFree = false);
	bool					HasExtraLeveledItem();
	void					RemoveCannotWear();
	float					GetItemHealthPerc(bool arg1 = true);
	ExtraDataList*			GetEquippedExtra();
	ExtraDataList*			GetCustomExtra(UInt32 whichVal);
	BSExtraData*			GetExtraData(UInt32 whichVal);
	float					CalculateWeaponDamage(float condition, TESForm* ammo);
	float					GetValue();
	bool					HasWeaponMod(UInt32 modEffect) { return ThisStdCall<bool>(0x4BDA70, this, modEffect); }
	UInt32					GetWeaponNumProjectiles(Actor* owner);
	bool					ShouldDisplay();

	UInt8					GetWeaponMod();
	__forceinline Float64	GetHealthPercent(char a1 = 0) { return ThisCall<Float64>(0x4BCDB0, this, a1); };
	Float64					GetHealthPercentAlt(bool axonisFix = false);
	bool					GetEquipped();
	ExtraDataList* GetExtraData()
	{
		return extendData ? extendData->first.data : nullptr;
	}
};
static_assert(sizeof(InventoryChanges) == 0xC);

struct InventoryChangesList : TList<InventoryChanges>
{
	InventoryChanges*				FindForItem(TESForm* item);
	void							Free(bool bFreeList);
	static InventoryChangesList*	Create(UInt32 refID, UInt32 count, ExtendDataList* pExtendDataList);
};

struct InventoryItemData
{
	SInt32								count;
	InventoryChanges* entry;

	InventoryItemData(SInt32 count, InventoryChanges* entry) : count(count), entry(entry) {}
};
static_assert(sizeof(InventoryItemData) == 0x08);

typedef std::vector<InventoryChanges*> InventoryChangesArray;
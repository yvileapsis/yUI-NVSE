#pragma once
#include <Sound.h>
#include <GameForms.h>

#define				GetByTypeCast(xDataList, Type) DYNAMIC_CAST(xDataList.GetByType(kExtraData_ ## Type), BSExtraData, Extra ## Type)
extern char*		GetExtraDataValue(BSExtraData* traverse);
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
	TESForm*		actionRef;	// 010

	static ExtraAction* Create();
};
static_assert(sizeof(ExtraAction) == 0x14);

class ExtraScript : public BSExtraData
{
public:
	ExtraScript();
	virtual ~ExtraScript();

	Script*				script;		// 00C
	ScriptEventList*	eventList;	// 010

	static ExtraScript*		Create(TESScriptableForm* baseForm = NULL, bool create = true, TESObjectREFR* container = NULL);
	void					EventCreate(UInt32 eventCode, TESObjectREFR* container);
};
static_assert(sizeof(ExtraScript) == 0x14);

struct ExtraContainerChangesData
{
	InventoryChangesList*	objList;	// 000
	TESObjectREFR*			owner;		// 004
	float					unk2;		// 008	OnEquip: at the begining, stored into unk3 and forced to -1.0 before checking owner
	float					unk3;		// 00C
	UInt8					byte10;		// 010	referenced in relation to scripts in container
	UInt8					pad[3];

	static ExtraContainerChangesData*	Create(TESObjectREFR* owner);
	Float64								GetInventoryWeight();
};

class ExtraContainerChanges : public BSExtraData
{
public:
	ExtraContainerChanges();
	virtual ~ExtraContainerChanges();

	// find the equipped item whose form matches the passed matcher
	struct FoundEquipData {
		TESForm*		pForm;
		ExtraDataList*	pExtraData;
	};

	ExtraContainerChangesData*		data;	// 00C

	InventoryChanges*				GetByType(TESForm * type);
	void							DebugDump();
	void							Cleanup();	// clean up unneeded extra data from each EntryData
	ExtendDataList*					Add(TESForm* form, ExtraDataList* dataList = NULL);
	bool							Remove(TESForm* form, ExtraDataList* dataList = NULL, bool bFree = false);
	ExtraDataList*					SetEquipped(TESForm* obj, bool bEquipped, bool bForce = false);
	// get EntryData and ExtendData for all equipped objects, return num objects equipped
	UInt32							GetAllEquipped(InventoryChangesArray& outEntryData, ExtendDataArray& outExtendData);
	static ExtraContainerChanges*	Create();
	static ExtraContainerChanges*	Create(TESObjectREFR* thisObj, UInt32 refID = 0, UInt32 count = 1,
	                                     ExtendDataList* pExtendDataList = NULL);
	static ExtraContainerChanges*	GetForRef(TESObjectREFR* refr);
	FoundEquipData					FindEquipped(FormMatcher& matcher) const;
	InventoryChangesList*			GetEntryDataList() const { return data ? data->objList : nullptr; }
};
static_assert(sizeof(ExtraContainerChanges) == 0x10);

class TileInventoryChangesUnk
{
public:
	Tile*				tile = nullptr;
	InventoryChanges*	entry = nullptr;
	UInt32				count = 0;

	TileInventoryChangesUnk() = default;
	TileInventoryChangesUnk(InventoryChanges* entry, Tile* tile = nullptr) : tile(tile), entry(entry), count(entry->countDelta) {}
};

// Finds an ExtraDataList in an ExtendDataList
class ExtraDataListInExtendDataListMatcher
{
	ExtraDataList* m_toMatch;
public:
	ExtraDataListInExtendDataListMatcher(ExtraDataList* match) : m_toMatch(match) { }

	bool Accept(ExtraDataList* match) { return (m_toMatch == match); }
};

// Finds an ExtraDataList in an ExtendDataList embedded in one of the EntryData from an EntryDataList
class ExtraDataListInEntryDataListMatcher
{
	ExtraDataList* m_toMatch;
public:
	ExtraDataListInEntryDataListMatcher(ExtraDataList* match) : m_toMatch(match) { }

	bool Accept(InventoryChanges* match);
};

// Finds an ExtendDataList in an EntryDataList
class ExtendDataListInEntryDataListMatcher
{
	ExtendDataList* m_toMatch;
public:
	ExtendDataListInEntryDataListMatcher(ExtendDataList* match) : m_toMatch(match) { }

	bool Accept(InventoryChanges* match) { return match && match->extendData ? match->extendData == m_toMatch : false; }
};

// Finds an EntryData in an EntryDataList
class EntryDataInEntryDataListMatcher
{
	InventoryChanges* m_toMatch;
public:
	EntryDataInEntryDataListMatcher(InventoryChanges* match) : m_toMatch(match) { }

	bool Accept(InventoryChanges* match) { return (m_toMatch == match); }
};

// Finds an Item (type) in an EntryDataList
class ItemInEntryDataListMatcher
{
	TESForm* m_toMatch;
public:
	ItemInEntryDataListMatcher(TESForm* match) : m_toMatch(match) { }

	bool Accept(InventoryChanges* match) { return (match && m_toMatch == match->form); }
};

// Finds an Item from its base form in an EntryDataList
class BaseInEntryDataLastMatcher
{
	TESForm* m_toMatch;
public:
	BaseInEntryDataLastMatcher(TESForm* match) : m_toMatch(match) { }

	bool Accept(InventoryChanges* match) { return (match && match->form && m_toMatch == match->form->TryGetREFRParent()); }
};

// Finds an item by refID in an EntryDataList
class RefIDInEntryDataListMatcher
{
	UInt32 m_toMatch;
public:
	RefIDInEntryDataListMatcher(UInt32 match) : m_toMatch(match) { }

	bool Accept(InventoryChanges* match) { return (match && match->form && m_toMatch == match->form->refID); }
};

// Finds an item by the refID of its base form in an EntryDataList
class BaseIDInEntryDataListMatcher
{
	UInt32 m_toMatch;
public:
	BaseIDInEntryDataListMatcher(UInt32 match) : m_toMatch(match) { }

	bool Accept(InventoryChanges* match) { return (match && match->form && match->form->TryGetREFRParent() && m_toMatch == match->form->TryGetREFRParent()->refID); }
};

typedef ExtraContainerChanges::FoundEquipData EquipData;

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

struct ExtraLockData
{
	enum
	{
		kLocked = 1
	};

	UInt32	lockLevel;	// 00
	TESKey* key;		// 04
	UInt8	flags;		// 08
	UInt8	pad[3];
	UInt32  unk0C;		// 0C introduced since form version 0x10
	UInt32	unk10;		// 10

	bool IsLocked() { return flags & kLocked; };
};

class ExtraLock : public BSExtraData
{
public:
	ExtraLock();
	virtual ~ExtraLock();

	ExtraLockData*	data;		// 00C

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

	struct Data
	{
		TESObjectREFR*	linkedDoor;	// 00
		float			x;			// 04 x, y, z, zRot refer to teleport marker's position and rotation
		float			y; 
		float			z;
		float			xRot;		// 10 angles in radians. x generally 0
		float			yRot;		// 14 y generally -0.0, no reason to modify
		float			zRot;		// 18
		UInt8			unk01C;		// 1C
		UInt8			pad01D[3];	// 1D
	};

	Data*	data;

	static ExtraTeleport* Create();
};
static_assert(sizeof(ExtraTeleport) == 0x10);

class ExtraRandomTeleportMarker : public BSExtraData
{
public:
	ExtraRandomTeleportMarker();
	~ExtraRandomTeleportMarker();

	TESObjectREFR *	teleportRef;
};
static_assert(sizeof(ExtraRandomTeleportMarker) == 0x10);

class ExtraAmmo : public BSExtraData
{
public:
	ExtraAmmo();
	~ExtraAmmo();

	TESAmmo*	ammo;	// 0xC
	UInt32		count;	// 0x10
};
static_assert(sizeof(ExtraAmmo) == 0x14);

class ExtraOwnership : public BSExtraData
{
public:
	ExtraOwnership();
	virtual ~ExtraOwnership();

	TESForm*	owner;	// maybe this should be a union {TESFaction*; TESNPC*} but it would be more unwieldy to access and modify

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

	TESGlobal*	globalVar;	// 00C
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

	struct FactionListData
	{
		TESFaction*		faction;
		UInt8			rank;
		UInt8			pad[3];
	};

	typedef TList<FactionListData> FactionListEntry;
	FactionListEntry*	data;

	void						DebugDump();
	static ExtraFactionChanges* Create();
};
static_assert(sizeof(ExtraFactionChanges) == 0x10);

class ExtraFactionChangesMatcher
{
	TESFaction* pFaction;
	ExtraFactionChanges* xFactionChanges;
public:
	ExtraFactionChangesMatcher(TESFaction* faction, ExtraFactionChanges* FactionChanges) : pFaction(faction), xFactionChanges(FactionChanges) {}
	bool Accept(const ExtraFactionChanges::FactionListData* data) { return data->faction == pFaction ? true : false; }
};

ExtraFactionChanges::FactionListEntry*	GetExtraFactionList(BaseExtraList& xDataList);
SInt8									GetExtraFactionRank(BaseExtraList& xDataList, TESFaction * faction);
void									SetExtraFactionRank(BaseExtraList& xDataList, TESFaction * faction, SInt8 rank);

class ExtraLeveledCreature : public BSExtraData
{
public:
	ExtraLeveledCreature();
	virtual ~ExtraLeveledCreature();

	TESActorBase*		baseForm;		// 00C
	TESActorBase*		actorBase;		// 010
};
static_assert(sizeof(ExtraLeveledCreature) == 0x14);

// PackageStartLocation = Worldspace or Cell / PosX / PosY / PosZ / and 4 bytes

class ExtraCombatStyle : public BSExtraData
{
public:
	ExtraCombatStyle();
	virtual ~ExtraCombatStyle();

	TESCombatStyle*		combatStyle;	// 00C
};
static_assert(sizeof(ExtraCombatStyle) == 0x10);

class ExtraReferencePointer : public BSExtraData
{
public:
	ExtraReferencePointer();
	virtual ~ExtraReferencePointer();

	TESObjectREFR*		refr;			// 00C
};
static_assert(sizeof(ExtraReferencePointer) == 0x10);

struct ExtraMapMarkerData
{
	TESFullName		fullName;            // not all markers have this
	UInt16			flags;
	UInt16			type;
	TESForm*		reputation;            // not all markers have this
};

// Provided by "Luthien Anarion"
class ExtraMapMarker : BSExtraData
{
public:
    ExtraMapMarker();
    ~ExtraMapMarker();

    enum
    {
        kFlag_Visible	= 1 << 0,        // shown on the world map
        kFlag_CanTravel	= 1 << 1,        // visited, can fast-travel to it
        kFlag_Hidden    = 1 << 2,        // does not appear with Explorer perk
    };
    enum
    {
        kType_None    = 0,                // this determines the icon on the world map
        kType_City,
        kType_Settlement,
        kType_Encampment,
        kType_NaturalLandmark,
        kType_Cave,
        kType_Factory,
        kType_Memorial,
        kType_Military,
        kType_Office,
        kType_TownRuins,
        kType_UrbanRuins,
        kType_SewerRuins,
        kType_Metro,
        kType_Vault,
    };

	ExtraMapMarkerData*	data;

    // flag member functions
	bool IsVisible()				{ return (data->flags & kFlag_Visible) == kFlag_Visible; }
	bool CanTravel()				{ return (data->flags & kFlag_CanTravel) == kFlag_CanTravel; }
	bool IsHidden()					{ return (data->flags & kFlag_Hidden) == kFlag_Hidden; }
	void SetVisible(bool visible)	{ data->flags = visible ? data->flags | kFlag_Visible : data->flags & ~kFlag_Visible; }
	void SetCanTravel(bool travel)	{ data->flags = travel ? data->flags | kFlag_CanTravel : data->flags & ~kFlag_CanTravel; }
	void SetHidden(bool hidden)		{ data->flags = hidden ? data->flags | kFlag_Hidden : data->flags & ~kFlag_Hidden; }
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
	ExtraIntSeenData*	next;		// 028
};
static_assert(sizeof(ExtraIntSeenData) == 0x2C);

// ExtraUsedMarkers is a bitmask of 30 bits.

class ExtraPersistentCell : public BSExtraData
{
public:
	ExtraPersistentCell();
	virtual ~ExtraPersistentCell();

	TESObjectCELL*		persistentCell;	// 0C
};
static_assert(sizeof(ExtraPersistentCell) == 0x10);

class ExtraTerminalState : public BSExtraData
{
public:
	ExtraTerminalState();
	virtual ~ExtraTerminalState();

    enum
    {
        kFlag_Locked	= 1 << 7,        // terminal is locked
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

	Animation*	data;	// 0C
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
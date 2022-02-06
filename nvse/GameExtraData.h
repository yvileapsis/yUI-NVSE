#pragma once

#include <GameBSExtraData.h>
#include <GameTypes.h>
#include <GameForms.h>
#include <GameSound.h>
#include <vector>

#define GetByTypeCast(xDataList, Type) DYNAMIC_CAST(xDataList.GetByType(kExtraData_ ## Type), BSExtraData, Extra ## Type)
extern char * GetExtraDataValue(BSExtraData* traverse);
extern const char * GetExtraDataName(UInt8 ExtraDataType);

// 018
class ExtraSound : public BSExtraData
{
public:
	ExtraSound();
	~ExtraSound();

	Sound	sound; // 00C
};

class ExtraDroppedItemList : public BSExtraData
{
public:
	tList<TESObjectREFR>	itemRefs;	// 0C
};


// 014
class ExtraAction : public BSExtraData
{
public:
	ExtraAction();
	virtual ~ExtraAction();

	UInt8			flags0C;	// 00C	some kind of status or flags: 
								//		if not RunOnActivateBlock: bit0 and bit1 are set before TESObjectREF::Activate, bit0 is preserved, bit1 is cleared after returning.
	UInt8			fill0D[3];	// 00D
	TESForm		*	actionRef;	// 010

	static ExtraAction* Create();
};

// 014
class ExtraScript : public BSExtraData
{
public:
	ExtraScript();
	virtual ~ExtraScript();

	Script			* script;		// 00C
	ScriptEventList	* eventList;	// 010

	static ExtraScript* Create(TESForm* baseForm = NULL, bool create = true, TESObjectREFR* container = NULL);
	void EventCreate(UInt32 eventCode, TESObjectREFR* container);
};

SInt32 GetCountForExtraDataList(ExtraDataList* list);

// 010
class ExtraContainerChanges : public BSExtraData
{
public:
	ExtraContainerChanges();
	virtual ~ExtraContainerChanges();

	class ExtendDataList: public tList<ExtraDataList>
	{
	public:
		SInt32				AddAt(ExtraDataList* item, SInt32 index);
		void				RemoveAll() const;
		ExtraDataList*		RemoveNth(SInt32 n);
	};

	struct EntryData
	{
		ExtendDataList*		extendData;		// 00
		SInt32				countDelta;		// 04
		TESForm*			type;			// 08

		void				Cleanup();
		static EntryData*	Create(UInt32 refID = 0, UInt32 count = 1, ExtraContainerChanges::ExtendDataList* pExtendDataList = NULL);
		static EntryData*	Create(TESForm* pForm, UInt32 count = 1, ExtraContainerChanges::ExtendDataList* pExtendDataList = NULL);
		ExtendDataList*		Add(ExtraDataList* newList);
		bool				Remove(ExtraDataList* toRemove, bool bFree = false);

		bool HasExtraLeveledItem()
		{
			if (!extendData) return false;
			for (auto iter = extendData->Begin(); !iter.End(); ++iter)
				if (iter->HasType(kExtraData_LeveledItem)) return true;
			return false;
		}

		void				RemoveCannotWear();
		float				GetItemHealthPerc(bool arg1 = true);
		ExtraDataList*		GetEquippedExtra();
		ExtraDataList*		GetCustomExtra(UInt32 whichVal);
		float				CalculateWeaponDamage(float condition, TESForm* ammo);
		float				GetValue();
	};

	struct EntryDataList : tList<EntryData>
	{
		EntryData *FindForItem(TESForm *item)
		{
			for (auto iter = Begin(); !iter.End(); ++iter)
				if (iter->type == item) return iter.Get();
			return NULL;
		}
	};

	typedef std::vector<EntryData*> DataArray;
	typedef std::vector<ExtendDataList*> ExtendDataArray;

	struct Data
	{
		EntryDataList	* objList;	// 000
		TESObjectREFR	* owner;	// 004
		float			unk2;		// 008	OnEquip: at the begining, stored into unk3 and forced to -1.0 before checking owner
		float			unk3;		// 00C
		UInt8			byte10;		// 010	referenced in relation to scripts in container
		UInt8			pad[3];

		static Data* Create(TESObjectREFR* owner);
	};

	Data	* data;	// 00C

	EntryData *	GetByType(TESForm * type);

	void DebugDump();
	void Cleanup();	// clean up unneeded extra data from each EntryData
	ExtendDataList * Add(TESForm* form, ExtraDataList* dataList = NULL);
	bool Remove(TESForm* form, ExtraDataList* dataList = NULL, bool bFree = false);
	ExtraDataList* SetEquipped(TESForm* obj, bool bEquipped, bool bForce = false);

	// get EntryData and ExtendData for all equipped objects, return num objects equipped
	UInt32 GetAllEquipped(DataArray& outEntryData, ExtendDataArray& outExtendData);

	static ExtraContainerChanges* Create();
	static ExtraContainerChanges* Create(TESObjectREFR* thisObj, UInt32 refID = 0, UInt32 count = 1,
		ExtraContainerChanges::ExtendDataList* pExtendDataList = NULL);
	static ExtraContainerChanges* GetForRef(TESObjectREFR* refr);

	// find the equipped item whose form matches the passed matcher
	struct FoundEquipData{
		TESForm* pForm;
		ExtraDataList* pExtraData;
	};
	FoundEquipData FindEquipped(FormMatcher& matcher) const;

	EntryDataList* GetEntryDataList() const {
		return data ? data->objList : NULL;
	}
};
typedef ExtraContainerChanges::DataArray ExtraContainerDataArray;
typedef ExtraContainerChanges::ExtendDataArray ExtraContainerExtendDataArray;
typedef ExtraContainerChanges::EntryData ContChangesEntry;

struct InventoryItemData
{
	SInt32								count;
	ExtraContainerChanges::EntryData	*entry;

	InventoryItemData(SInt32 _count, ExtraContainerChanges::EntryData *_entry) : count(_count), entry(_entry) {}
};

typedef std::unordered_map<TESForm*, InventoryItemData> InventoryItemsMap;

// Finds an ExtraDataList in an ExtendDataList
class ExtraDataListInExtendDataListMatcher
{
	ExtraDataList* m_toMatch;
public:
	ExtraDataListInExtendDataListMatcher(ExtraDataList* match) : m_toMatch(match) { }

	bool Accept(ExtraDataList* match)
	{
		return (m_toMatch == match);
	}
};

// Finds an ExtraDataList in an ExtendDataList embedded in one of the EntryData from an EntryDataList
class ExtraDataListInEntryDataListMatcher
{
	ExtraDataList* m_toMatch;
public:
	ExtraDataListInEntryDataListMatcher(ExtraDataList* match) : m_toMatch(match) { }

	bool Accept(ExtraContainerChanges::EntryData* match);
};

// Finds an ExtendDataList in an EntryDataList
class ExtendDataListInEntryDataListMatcher
{
	ExtraContainerChanges::ExtendDataList* m_toMatch;
public:
	ExtendDataListInEntryDataListMatcher(ExtraContainerChanges::ExtendDataList* match) : m_toMatch(match) { }

	bool Accept(ExtraContainerChanges::EntryData* match)
	{
		if (match && match->extendData)
			return (match->extendData == m_toMatch);
		else
			return false;
	}
};

// Finds an EntryData in an EntryDataList
class EntryDataInEntryDataListMatcher
{
	ExtraContainerChanges::EntryData* m_toMatch;
public:
	EntryDataInEntryDataListMatcher(ExtraContainerChanges::EntryData* match) : m_toMatch(match) { }

	bool Accept(ExtraContainerChanges::EntryData* match)
	{
		return (m_toMatch == match);
	}
};

// Finds an Item (type) in an EntryDataList
class ItemInEntryDataListMatcher
{
	TESForm* m_toMatch;
public:
	ItemInEntryDataListMatcher(TESForm* match) : m_toMatch(match) { }

	bool Accept(ExtraContainerChanges::EntryData* match)
	{
		return (match && m_toMatch == match->type);
	}
};

// Finds an Item from its base form in an EntryDataList
class BaseInEntryDataLastMatcher
{
	TESForm* m_toMatch;
public:
	BaseInEntryDataLastMatcher(TESForm* match) : m_toMatch(match) { }

	bool Accept(ExtraContainerChanges::EntryData* match)
	{
		return (match && match->type && m_toMatch == match->type->TryGetREFRParent());
	}
};

// Finds an item by refID in an EntryDataList
class RefIDInEntryDataListMatcher
{
	UInt32 m_toMatch;
public:
	RefIDInEntryDataListMatcher(UInt32 match) : m_toMatch(match) { }

	bool Accept(ExtraContainerChanges::EntryData* match)
	{
		return (match && match->type && m_toMatch == match->type->refID);
	}
};

// Finds an item by the refID of its base form in an EntryDataList
class BaseIDInEntryDataListMatcher
{
	UInt32 m_toMatch;
public:
	BaseIDInEntryDataListMatcher(UInt32 match) : m_toMatch(match) { }

	bool Accept(ExtraContainerChanges::EntryData* match)
	{
		return (match && match->type && match->type->TryGetREFRParent() && m_toMatch == match->type->TryGetREFRParent()->refID);
	}
};

typedef ExtraContainerChanges::FoundEquipData EquipData;

extern ExtraContainerChanges::ExtendDataList* ExtraContainerChangesExtendDataListCreate(ExtraDataList* pExtraDataList = NULL);
extern void ExtraContainerChangesExtendDataListFree(ExtraContainerChanges::ExtendDataList* xData, bool bFreeList = false);
extern bool ExtraContainerChangesExtendDataListAdd(ExtraContainerChanges::ExtendDataList* xData, ExtraDataList* xList);
extern bool ExtraContainerChangesExtendDataListRemove(ExtraContainerChanges::ExtendDataList* xData, ExtraDataList* xList, bool bFreeList = false);

extern void ExtraContainerChangesEntryDataFree(ExtraContainerChanges::EntryData* xData, bool bFreeList = false);

extern ExtraContainerChanges::EntryDataList* ExtraContainerChangesEntryDataListCreate(UInt32 refID = 0, UInt32 count = 1, ExtraContainerChanges::ExtendDataList* pExtendDataList = NULL);
extern void ExtraContainerChangesEntryDataListFree(ExtraContainerChanges::EntryDataList* xData, bool bFreeList = false);

extern void ExtraContainerChangesFree(ExtraContainerChanges* xData, bool bFreeList = false);

// 010
class ExtraHealth : public BSExtraData
{
public:
	ExtraHealth();
	virtual ~ExtraHealth();
	float health;

	static ExtraHealth* Create();
};

// 00C
class ExtraWorn : public BSExtraData	// Item is equipped
{
public:
	ExtraWorn();
	virtual ~ExtraWorn();

	static ExtraWorn* Create();
};

// 00C
class ExtraWornLeft : public BSExtraData	// haven't seen used yet
{
public:
	ExtraWornLeft();
	virtual ~ExtraWornLeft();

	//static ExtraWornLeft* Create();
};

// 00C
class ExtraCannotWear : public BSExtraData	//	Seen used as ForceEquip ! Unused as forceUnequip (bug?)
{
public:
	ExtraCannotWear();
	virtual ~ExtraCannotWear();

	static ExtraCannotWear* Create();
};

// 010
class ExtraHotkey : public BSExtraData
{
public:
	ExtraHotkey();
	virtual ~ExtraHotkey();

	UInt8	index;		// 00C (is 0-7)

	static ExtraHotkey* Create();
};

// 010
class ExtraCount : public BSExtraData
{
public:
	ExtraCount();
	virtual ~ExtraCount();

	SInt16	count;	// 00C
	UInt8	pad[2];	// 00E

	static ExtraCount* Create(UInt32 count = 0);	
};

// 010
class ExtraLock : public BSExtraData
{
public:
	ExtraLock();
	virtual ~ExtraLock();

	struct Data
	{
		UInt32	lockLevel;	// 00
		TESKey	* key;		// 04
		UInt8	flags;		// 08
		UInt8	pad[3];
		UInt32  unk0C;		// 0C introduced since form version 0x10
		UInt32	unk10;		// 10
	};

	Data*	data;		// 00C

	static ExtraLock* Create();
};

// 010
class ExtraUses : public BSExtraData
{
public:
	ExtraUses();
	~ExtraUses();

	UInt32 unk0;

	static ExtraUses* Create();
};

// 010
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

	Data *	data;

	static ExtraTeleport* Create();
};

// 010
class ExtraRandomTeleportMarker : public BSExtraData
{
public:
	ExtraRandomTeleportMarker();
	~ExtraRandomTeleportMarker();

	TESObjectREFR *	teleportRef;
};

// 014
class ExtraAmmo : public BSExtraData
{
public:
	ExtraAmmo();
	~ExtraAmmo();

	TESAmmo* ammo;
	UInt32 unk4;
};

// 010
class ExtraOwnership : public BSExtraData
{
public:
	ExtraOwnership();
	virtual ~ExtraOwnership();

	TESForm	* owner;	// maybe this should be a union {TESFaction*; TESNPC*} but it would be more unwieldy to access and modify

	static ExtraOwnership* Create();
};

// 010
class ExtraRank : public BSExtraData
{
public:
	ExtraRank();
	virtual ~ExtraRank();

	UInt32	rank; // 00C

	static ExtraRank* Create();
};

// 010
class ExtraGlobal : public BSExtraData
{								//ownership data, stored separately from ExtraOwnership
public:
	ExtraGlobal();
	virtual ~ExtraGlobal();

	TESGlobal*	globalVar;	// 00C
};

// 010
class ExtraWeaponModFlags : public BSExtraData
{
public:
	ExtraWeaponModFlags();
	~ExtraWeaponModFlags();

	UInt8	flags; // 00C

	static ExtraWeaponModFlags* Create();
};

class ExtraFactionChanges : public BSExtraData
{
public:
	ExtraFactionChanges();
	virtual ~ExtraFactionChanges();

	struct FactionListData
	{
		TESFaction	* faction;
		UInt8		rank;
		UInt8		pad[3];
	};

	typedef tList<FactionListData> FactionListEntry;
	FactionListEntry* data;

	void		DebugDump();

	static ExtraFactionChanges* Create();
};

STATIC_ASSERT(sizeof(ExtraFactionChanges) == 0x10);

class ExtraFactionChangesMatcher
{
	TESFaction* pFaction;
	ExtraFactionChanges* xFactionChanges;
public:
	ExtraFactionChangesMatcher(TESFaction* faction, ExtraFactionChanges* FactionChanges) : pFaction(faction), xFactionChanges(FactionChanges) {}
	bool Accept(ExtraFactionChanges::FactionListData* data) {
		return (data->faction == pFaction) ? true : false;
	}
};

ExtraFactionChanges::FactionListEntry* GetExtraFactionList(BaseExtraList& xDataList);
SInt8 GetExtraFactionRank(BaseExtraList& xDataList, TESFaction * faction);
void SetExtraFactionRank(BaseExtraList& xDataList, TESFaction * faction, SInt8 rank);

class ExtraLeveledCreature : public BSExtraData
{
public:
	ExtraLeveledCreature();
	virtual ~ExtraLeveledCreature();

	TESActorBase* baseForm;	// 00C
	TESActorBase* actorBase;		// 010
};

STATIC_ASSERT(sizeof(ExtraLeveledCreature) == 0x14);

// PackageStartLocation = Worldspace or Cell / PosX / PosY / PosZ / and 4 bytes

class ExtraCombatStyle : public BSExtraData
{
public:
	ExtraCombatStyle();
	virtual ~ExtraCombatStyle();

	TESCombatStyle*	combatStyle;		// 00C
};

class ExtraReferencePointer : public BSExtraData
{
public:
	ExtraReferencePointer();
	virtual ~ExtraReferencePointer();

	TESObjectREFR* refr;		// 00C
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

    struct MarkerData
    {
        TESFullName fullName;            // not all markers have this
        UInt16 flags;
        UInt16 type;
        TESForm* reputation;            // not all markers have this
    };
    MarkerData    *data;

    // flag member functions
    bool IsVisible() {return (data->flags & kFlag_Visible) == kFlag_Visible;}
    bool CanTravel() {return (data->flags & kFlag_CanTravel) == kFlag_CanTravel;}
    bool IsHidden() {return (data->flags & kFlag_Hidden) == kFlag_Hidden;}
    void SetVisible(bool visible) {data->flags = (visible) ? (data->flags | kFlag_Visible) : (data->flags & ~kFlag_Visible);}
    void SetCanTravel(bool travel) {data->flags = (travel) ? (data->flags | kFlag_CanTravel) : (data->flags & ~kFlag_CanTravel);}
    void SetHidden(bool hidden) {data->flags = (hidden) ? (data->flags | kFlag_Hidden) : (data->flags & ~kFlag_Hidden);}
};

class ExtraNorthRotation : public BSExtraData
{
public:
	ExtraNorthRotation();
	virtual ~ExtraNorthRotation();

	UInt32 angle;		// 00C
};

class ExtraSeenData : public BSExtraData
{
public:
	ExtraSeenData();
	virtual ~ExtraSeenData();

	UInt8 unk[0x24 - 0x0C];		// 00C
};

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

// ExtraUsedMarkers is a bitmask of 30 bits.

// 10
class ExtraPersistentCell : public BSExtraData
{
public:
	ExtraPersistentCell();
	virtual ~ExtraPersistentCell();

	TESObjectCELL	*persistentCell;	// 0C
};

STATIC_ASSERT(sizeof(ExtraPersistentCell) == 0x10);

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

class ExtraAnim : public BSExtraData
{
public:
	ExtraAnim();
	virtual ~ExtraAnim();

	struct Animation
	{
	};	// 013C

	Animation* data;	// 0C
};	// 10

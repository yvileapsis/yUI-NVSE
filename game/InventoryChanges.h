#pragma once
#include <containers.h>
#include <Utilities.h>
#include <Form.h>

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

	struct WithExtraData
	{
		InventoryChanges* entry;
		ExtraDataList* extra;
	};

	void						Free(bool bFreeList = false);
	void						Cleanup();
	static InventoryChanges*	Create(TESForm* pForm, UInt32 count = 1, ExtendDataList* pExtendDataList = nullptr);
	ExtendDataList*				Add(ExtraDataList* newList);
	bool						Remove(ExtraDataList* toRemove, bool bFree = false);
	bool						HasExtraLeveledItem();
	void						RemoveCannotWear();
	ExtraDataList*				GetEquippedExtra();
	ExtraDataList*				GetCustomExtra(UInt32 whichVal);
	BSExtraData*				GetExtraData(UInt32 whichVal);
	float						CalculateWeaponDamage(float condition, TESForm* ammo);
	float						GetValue();
	bool						HasWeaponMod(UInt32 modEffect) { return ThisStdCall<bool>(0x4BDA70, this, modEffect); }
	UInt32						GetWeaponNumProjectiles(Actor* owner);
	bool						ShouldDisplay();

	UInt8						GetWeaponMod();
	__forceinline Float64		GetHealthPercent(char a1 = 0) { return ThisCall<Float64>(0x4BCDB0, this, a1); };
	Float64						GetHealthPercentAlt(bool axonisFix = false, bool checkDestruction = true);
	bool						GetEquipped();
	ExtraDataList*				GetExtraData() const;
	UInt32						GetClipSize();
	void						Equip(Actor* actor, ExtraDataList* extra = nullptr);

	enum
	{
		kHotkeyMin = 0,
		kHotkey0 = kHotkeyMin,
		kHotkey1,
		kHotkey2,
		kHotkey3,
		kHotkey4,
		kHotkey5,
		kHotkey6,
		kHotkey7,
		kHotkeyMax = kHotkey7,
		kHotkey8,
		kHotkeyStewie = kHotkey8
	};

	static WithExtraData	HotkeyGet(const UInt8 hotkey);
	static WithExtraData	HotkeyClear(const UInt8 hotkey);
	WithExtraData			HotkeySet(const UInt8 hotkey);
};
static_assert(sizeof(InventoryChanges) == 0xC);

struct InventoryChangesList : TList<InventoryChanges>
{
	InventoryChanges* FindForItem(TESForm* item);
	void							Free(bool bFreeList);
	static InventoryChangesList* Create(UInt32 refID, UInt32 count, ExtendDataList* pExtendDataList);
};

struct InventoryItemData
{
	SInt32								count;
	InventoryChanges* entry;

	InventoryItemData(SInt32 count, InventoryChanges* entry) : count(count), entry(entry) {}
};
static_assert(sizeof(InventoryItemData) == 0x08);

typedef std::vector<InventoryChanges*> InventoryChangesArray;


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


	void							Cleanup();	// clean up unneeded extra data from each EntryData
	ExtendDataList* Add(TESForm* form, ExtraDataList* dataList = NULL);
	bool							Remove(TESForm* form, ExtraDataList* dataList = NULL, bool bFree = false);
	ExtraDataList* SetEquipped(TESForm* obj, bool bEquipped, bool bForce = false);
	// get EntryData and ExtendData for all equipped objects, return num objects equipped
	UInt32							GetAllEquipped(InventoryChangesArray& outEntryData);

	static ExtraContainerChanges* GetForRef(TESObjectREFR* refr);

};

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

struct ExtraTeleportData
{
	TESObjectREFR* linkedDoor;	// 00
	float			x;			// 04 x, y, z, zRot refer to teleport marker's position and rotation
	float			y;
	float			z;
	float			xRot;		// 10 angles in radians. x generally 0
	float			yRot;		// 14 y generally -0.0, no reason to modify
	float			zRot;		// 18
	UInt8			unk01C;		// 1C
	UInt8			pad01D[3];	// 1D
};

struct FactionListEntry
{
	TESFaction* faction;
	UInt8			rank;
	UInt8			pad[3];
};

typedef TList<FactionListEntry> FactionListData;

struct ExtraMapMarkerData
{

	enum
	{
		kFlag_Visible = 1 << 0,        // shown on the world map
		kFlag_CanTravel = 1 << 1,        // visited, can fast-travel to it
		kFlag_Hidden = 1 << 2,        // does not appear with Explorer perk
	};
	enum
	{
		kType_None = 0,                // this determines the icon on the world map
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

	TESFullName		fullName;            // not all markers have this
	UInt16			flags;
	UInt16			type;
	TESForm* reputation;            // not all markers have this

	// flag member functions
	bool IsVisible() { return (flags & kFlag_Visible) == kFlag_Visible; }
	bool CanTravel() { return (flags & kFlag_CanTravel) == kFlag_CanTravel; }
	bool IsHidden() { return (flags & kFlag_Hidden) == kFlag_Hidden; }
	void SetVisible(bool visible) { flags = visible ? flags | kFlag_Visible : flags & ~kFlag_Visible; }
	void SetCanTravel(bool travel) { flags = travel ? flags | kFlag_CanTravel : flags & ~kFlag_CanTravel; }
	void SetHidden(bool hidden) { flags = hidden ? flags | kFlag_Hidden : flags & ~kFlag_Hidden; }
};
#pragma once
#include "BSSimpleList.hpp"

class ExtraDataList;
class TESBoundObject;
class Actor;

class ItemChange
{
public:

	BSSimpleList<ExtraDataList*>*	pkExtraLists;		// 00
	SInt32							iCountDelta;		// 04
	TESBoundObject*					pkObject;			// 08

	void						Free(bool bFreeList = false);
	void						Cleanup();
	static ItemChange*			Create(TESForm* pForm, UInt32 count = 1, BSSimpleList<ExtraDataList*>* pExtendDataList = nullptr);
	BSSimpleList<ExtraDataList*>*				Add(ExtraDataList* newList);
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
};
static_assert(sizeof(ItemChange) == 0xC);

//typedef std::vector<ExtendDataList*> ExtendDataArray;

class InventoryChanges
{
public:
	BSSimpleList<ItemChange*>*			pkEntryList;	// 000
	TESObjectREFR*						pkOwner;		// 004
	Float32								fTotalWgCurrent;
	Float32								fTotalWgLast;	// armor in sse
	UInt8								bChanged;		// 010	referenced in relation to scripts in container
	UInt8								pad[3];

	void								Cleanup();	// clean up unneeded extra data from each EntryData
};
static_assert(sizeof(InventoryChanges) == 0x14);

struct InventoryItemData
{
	SInt32				iCount;
	ItemChange*			pkItem;

	InventoryItemData(SInt32 count, ItemChange* entry) : iCount(count), pkItem(entry) {}
};
static_assert(sizeof(InventoryItemData) == 0x08);
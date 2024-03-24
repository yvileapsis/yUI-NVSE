#pragma once

#include "BSExtraData.hpp"
#include "ExtraDataList.hpp"
#include "TESForm.hpp"
#include "map"

class TESObjectREFR;

class ExtraContainerChanges : public BSExtraData {
public:
	ExtraContainerChanges();
	virtual ~ExtraContainerChanges();

	EXTRADATATYPE(CONTAINERCHANGES);

	class ExtendDataList : public BSSimpleList<ExtraDataList*>
	{
	public:
		SInt32 AddAt(ExtraDataList* item, SInt32 index);
		void RemoveAll() const;
		ExtraDataList* RemoveNth(SInt32 n);
	};

	struct EntryData
	{
		ExtendDataList* extendData;
		SInt32			countDelta;
		TESForm*		type;

		void Cleanup();
		static EntryData* Create(UInt32 refID = 0, UInt32 count = 1, ExtraContainerChanges::ExtendDataList* pExtendDataList = NULL);
		static EntryData* Create(TESForm* pForm, UInt32 count = 1, ExtraContainerChanges::ExtendDataList* pExtendDataList = NULL);
		ExtendDataList* Add(ExtraDataList* newList);
		bool Remove(ExtraDataList* toRemove, bool bFree = false);

		bool HasExtraLeveledItem() const {
			if (!extendData) 
				return false;

			extendData->Find([&](const BSSimpleList<ExtraDataList*>* extendData) {
				return extendData->GetItem()->HasType(ED_LEVELEDITEM);
			});

			return false;
		}
	};

	struct EntryDataList : BSSimpleList<EntryData*> {
		EntryData* FindForItem(TESForm* item) const {
			auto entry = Find([&](const BSSimpleList<EntryData*>* entry) {
				return entry->GetItem()->type == item;
			});

			return entry ? entry->GetItem() : nullptr;
		}
	};

	typedef std::vector<EntryData*> DataArray;
	typedef std::vector<ExtendDataList*> ExtendDataArray;

	struct Data
	{
		EntryDataList* objList;	// 000
		TESObjectREFR* owner;	// 004
		float			unk2;		// 008	OnEquip: at the begining, stored into unk3 and forced to -1.0 before checking owner
		float			unk3;		// 00C
		UInt8			byte10;		// 010	referenced in relation to scripts in container
		UInt8			pad[3];

		static Data* Create(TESObjectREFR* owner);
	};

	Data* data;	// 00C

	EntryData* GetByType(TESForm* type);

	void DebugDump();
	void Cleanup();	// clean up unneeded extra data from each EntryData
	ExtendDataList* Add(TESForm* form, ExtraDataList* dataList = NULL);
	bool Remove(TESForm* form, ExtraDataList* dataList = NULL, bool bFree = false);
	ExtraDataList* SetEquipped(TESForm* obj, bool bEquipped, bool bForce = false);

	// get EntryData and ExtendData for all equipped objects, return num objects equipped
	UInt32 GetAllEquipped(DataArray& outEntryData, ExtendDataArray& outExtendData);

	static ExtraContainerChanges* Create();
	static ExtraContainerChanges* Create(TESObjectREFR* thisObj, UInt32 refID = 0, UInt32 count = 1,
		ExtraContainerChanges::ExtendDataList* pExtendDataList = NULL);
	static ExtraContainerChanges* GetForRef(TESObjectREFR* refr);

	// find the equipped item whose form matches the passed matcher
	struct FoundEquipData {
		TESForm* pForm;
		ExtraDataList* pExtraData;
	};
//	FoundEquipData FindEquipped(FormMatcher& matcher) const;

	EntryDataList* GetEntryDataList() const {
		return data ? data->objList : NULL;
	}
};

typedef ExtraContainerChanges::DataArray ExtraContainerDataArray;
typedef ExtraContainerChanges::ExtendDataArray ExtraContainerExtendDataArray;
typedef ExtraContainerChanges::FoundEquipData EquipData;

extern ExtraContainerChanges::ExtendDataList* ExtraContainerChangesExtendDataListCreate(ExtraDataList* pExtraDataList = NULL);
extern void ExtraContainerChangesExtendDataListFree(ExtraContainerChanges::ExtendDataList* xData, bool bFreeList = false);
extern bool ExtraContainerChangesExtendDataListAdd(ExtraContainerChanges::ExtendDataList* xData, ExtraDataList* xList);
extern bool ExtraContainerChangesExtendDataListRemove(ExtraContainerChanges::ExtendDataList* xData, ExtraDataList* xList, bool bFreeList = false);

extern void ExtraContainerChangesEntryDataFree(ExtraContainerChanges::EntryData* xData, bool bFreeList = false);

extern ExtraContainerChanges::EntryDataList* ExtraContainerChangesEntryDataListCreate(UInt32 refID = 0, UInt32 count = 1, ExtraContainerChanges::ExtendDataList* pExtendDataList = NULL);
extern void ExtraContainerChangesEntryDataListFree(ExtraContainerChanges::EntryDataList* xData, bool bFreeList = false);

extern void ExtraContainerChangesFree(ExtraContainerChanges* xData, bool bFreeList = false);

#define GetByTypeCast(xDataList, Type) DYNAMIC_CAST(xDataList.GetExtraData(Type), BSExtraData, Extra ## Type)
extern char* GetExtraDataValue(BSExtraData* traverse);
extern const char* GetExtraDataName(UInt8 ExtraDataType);

SInt32 GetCountForExtraDataList(ExtraDataList* list);

struct InventoryItemData {
	SInt32								count;
	ExtraContainerChanges::EntryData*	entry;

	InventoryItemData(SInt32 _count, ExtraContainerChanges::EntryData* _entry) : count(_count), entry(_entry) {}
};

typedef std::map<TESForm*, InventoryItemData> InventoryItemsMap;
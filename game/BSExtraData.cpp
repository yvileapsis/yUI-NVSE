#include <BSExtraData.h>

#include <Form.h>
#include <Object.h>
#include <Reference.h>
#include <Script.h>
#include <GameRTTI.h>

static const UInt32 s_ExtraDataListVtbl							= 0x010143E8;	//	0x0100e3a8;

bool BaseExtraList::HasType(UInt32 type) const
{
	return (m_presenceBitfield[type >> 3] & 1 << (type & 7)) != 0;
}

void BaseExtraList::MarkType(UInt32 type, bool bCleared)
{
	UInt8 bitMask = 1 << (type & 7);
	UInt8 &flag = m_presenceBitfield[type >> 3];
	if (bCleared) flag &= ~bitMask;
	else flag |= bitMask;
}

ExtraDataList* ExtraDataList::Create(BSExtraData* xBSData)
{
	ExtraDataList* xData = (ExtraDataList*)FormHeapAlloc(sizeof(ExtraDataList));
	memset(xData, 0, sizeof(ExtraDataList));
	((UInt32*)xData)[0] = s_ExtraDataListVtbl;
	if (xBSData) xData->Add(xBSData);
	return xData;
}


ExtendDataList* ExtendDataList::Create(ExtraDataList* pExtraDataList)
{
	auto xData = (ExtendDataList*)FormHeapAlloc(sizeof(ExtendDataList));
	if (xData) {
		memset(xData, 0, sizeof(ExtendDataList));
		if (pExtraDataList)
			if (xData->AddAt(pExtraDataList, eListEnd) == eListInvalid) {
				FormHeapFree(xData);
				xData = NULL;
			}
	}
	return xData;
}

void ExtendDataList::Free(ExtendDataList* xData, bool bFreeList)
{
	if (xData)
	{
		if (bFreeList)
		{
			UInt32 i = 0;
			ExtraDataList* pExtraDataList = xData->GetNth(i);
			while (pExtraDataList)
			{
				for (UInt32 j = 0; j < 0xFF; j++)
					pExtraDataList->RemoveByType(j);
				i++;
				pExtraDataList = xData->GetNth(i);
			}
		}
		FormHeapFree(xData);
	}
}

bool ExtendDataList::Add(ExtendDataList* xData, ExtraDataList* xList) {
	if (xData && xList) {
		xData->AddAt(xList, eListEnd);
		return true;
	}
	return false;
}

bool ExtendDataList::Remove(ExtendDataList* xData, ExtraDataList* xList, bool bFreeList) {
	if (xData && xList) {
		UInt32 i = 0;
		ExtraDataList* pExtraDataList = xData->GetNth(i);
		while (pExtraDataList && pExtraDataList != xList) {
			i++;
			pExtraDataList = xData->GetNth(i);
		}
		if (pExtraDataList == xList) {
			xData->RemoveNth(i);
			if (bFreeList) {
				for (UInt32 j = 0; j < 0xFF; j++)
					pExtraDataList->RemoveByType(j);
				FormHeapFree(xList);
			}
			return true;
		}
	}
	return false;
}

bool BaseExtraList::MarkScriptEvent(UInt32 eventMask, TESForm* eventTarget)
{
	return MarkBaseExtraListScriptEvent(eventTarget, this, eventMask);
}

void InventoryChanges::Free(bool bFreeList) {
	if (this->extendData) {
		ExtendDataList::Free(this->extendData, bFreeList);
	}
	FormHeapFree(this);
}


// Finds an Item (type) in an EntryDataList
class ItemInEntryDataListMatcher
{
	TESForm* m_toMatch;
public:
	ItemInEntryDataListMatcher(TESForm* match) : m_toMatch(match) { }

	bool Accept(InventoryChanges* match) { return (match && m_toMatch == match->form); }
};


class ExtraFactionChangesMatcher
{
	TESFaction* pFaction;
	ExtraFactionChanges* xFactionChanges;
public:
	ExtraFactionChangesMatcher(TESFaction* faction, ExtraFactionChanges* FactionChanges) : pFaction(faction), xFactionChanges(FactionChanges) {}
	bool Accept(const FactionListEntry* data) { return data->faction == pFaction; }
};

// Finds an ExtendDataList in an EntryDataList
class ExtendDataListInEntryDataListMatcher
{
	ExtendDataList* m_toMatch;
public:
	ExtendDataListInEntryDataListMatcher(ExtendDataList* match) : m_toMatch(match) { }

	bool Accept(InventoryChanges* match) { return match && match->extendData ? match->extendData == m_toMatch : false; }
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


void InventoryChanges::Cleanup()
{
	// Didn't find the hook, let's fake it
	if (!extendData) return;

	for (const auto iter : *extendData)
	{
		if (const auto xCount = reinterpret_cast<ExtraCount*>(iter->GetByType(kExtraData_Count)); xCount &&
			xCount->count <= 1)
			iter->RemoveByType(kExtraData_Count);

		if (!countDelta && !iter->m_data) // There are other extras than count like ExtraWorn :)
			extendData->RemoveItem(iter);
	}
	extendData->RemoveItem(nullptr);
}

InventoryChanges* InventoryChanges::Create(TESForm* pForm, UInt32 count, ExtendDataList* pExtendDataList)
{
	const auto xData = static_cast<InventoryChanges*>(FormHeapAlloc(sizeof(InventoryChanges)));
	if (xData) {
		memset(xData, 0, sizeof(InventoryChanges));
		if (pForm) {
			xData->form = pForm;
			xData->countDelta = count;
			if (pExtendDataList == nullptr)
			{
				pExtendDataList = static_cast<ExtendDataList*>(FormHeapAlloc(sizeof(ExtendDataList)));
				if (pExtendDataList) memset(pExtendDataList, 0, sizeof(ExtendDataList));
			}
			xData->extendData = pExtendDataList;
		}
	}
	return xData;
}

ExtendDataList* InventoryChanges::Add(ExtraDataList* newList)
{
	if (extendData)
		extendData->AddAt(newList, eListEnd);
	else
		extendData = ExtendDataList::Create(newList);
	const auto xCount = (ExtraCount*)newList->GetByType(kExtraData_Count);
	countDelta += xCount ? xCount->count : 1;
	return extendData;
}

bool InventoryChanges::Remove(ExtraDataList* toRemove, bool bFree)
{
	if (extendData && toRemove) {
		const SInt32 index = extendData->GetIndexOfItem(toRemove);
		if (index >= 0) {
			const auto xCount = (ExtraCount*)toRemove->GetByType(kExtraData_Count);
			const SInt16 count = xCount ? xCount->count : 1;

			extendData->RemoveNth(index);
			countDelta -= count;
			if (bFree) {
				toRemove->RemoveAll(true);
				FormHeapFree(toRemove);
			}
			return true;
		}

	}
	return false;
}

bool InventoryChanges::HasExtraLeveledItem()
{
	if (!extendData) return false;
	for (const auto iter : *extendData)	if (iter->HasType(kExtraData_LeveledItem)) return true;
	return false;
}


ExtraDataList* InventoryChanges::GetEquippedExtra()
{
	return GetCustomExtra(kExtraData_Worn);
}

ExtraDataList* InventoryChanges::GetCustomExtra(UInt32 whichVal)
{
	if (!extendData) return nullptr;
	const TListNode<ExtraDataList>* xdlIter = extendData->Head();
	do if (const auto xData = xdlIter->data; xData && xData->HasType(whichVal)) return xData;
	while ((xdlIter = xdlIter->next));
	return nullptr;
}

BSExtraData* InventoryChanges::GetExtraData(UInt32 whichVal)
{
	const auto extra = GetCustomExtra(whichVal);
	return extra ? extra->GetByType(whichVal) : nullptr;
}

UInt32 InventoryChanges::GetWeaponNumProjectiles(Actor* owner)
{
	return ThisCall<UInt8>(0x525B20, this->form, this->HasWeaponMod(0xC), 0, owner);
}

bool InventoryChanges::ShouldDisplay()
{
	return this->form->IsItemPlayable() && *this->form->GetTheName();
}

UInt8 InventoryChanges::GetWeaponMod()
{
	const auto xModFlags = reinterpret_cast<ExtraWeaponModFlags*>(this->GetExtraData(kExtraData_WeaponModFlags));
	return xModFlags ? xModFlags->flags : 0;
}

Float64 InventoryChanges::GetHealthPercentAlt(bool axonisFix)
{
	Float64 healthPer = -1;

	if (const auto healthForm = DYNAMIC_CAST(form->TryGetREFRParent(), TESForm, TESHealthForm))
	{
		const auto xHealth = form->GetIsReference() ? reinterpret_cast<ExtraHealth*>(reinterpret_cast<TESObjectREFR*>(form)->extraDataList.GetByType(kExtraData_Health)) : reinterpret_cast<ExtraHealth*>(this->GetExtraData(kExtraData_Health));

		healthPer = xHealth ? xHealth->health / ((int)healthForm->health + (
			HasWeaponMod(TESObjectWEAP::kWeaponModEffect_IncreaseMaxCondition)
			? reinterpret_cast<TESObjectWEAP*>(form)->GetEffectModValue(TESObjectWEAP::kWeaponModEffect_IncreaseMaxCondition)
			: 0))
			: 1;
	}
	else
	{
		const auto destructible = DYNAMIC_CAST(form->TryGetREFRParent(), TESForm, BGSDestructibleObjectForm);
		if (destructible && destructible->data)
		{
			const auto xObjHealth = form->GetIsReference() ? reinterpret_cast<ExtraObjectHealth*>(reinterpret_cast<TESObjectREFR*>(form)->extraDataList.GetByType(kExtraData_ObjectHealth)) : reinterpret_cast<ExtraObjectHealth*>(this->GetExtraData(kExtraData_ObjectHealth));
			healthPer = xObjHealth ? xObjHealth->health / (int)destructible->data->health : 1;
		}
	}
	return axonisFix ? healthPer >= 0.995 ? 1 : healthPer >= 95 ? 0.95 : healthPer : healthPer;
}

bool InventoryChanges::GetEquipped()
{
	if (reinterpret_cast<ExtraWorn*>(GetExtraData(kExtraData_Worn))) return true;
	if (reinterpret_cast<ExtraWornLeft*>(GetExtraData(kExtraData_WornLeft))) return true;
	return false;
}

ExtraDataList* InventoryChanges::GetExtraData() const { return extendData ? extendData->first.data : nullptr; }

__declspec(naked) InventoryChanges* InventoryChangesList::FindForItem(TESForm* item)
{
	__asm
	{
		mov		edx, [esp + 4]
	listIter:
		mov		eax, [ecx]
		test	eax, eax
		jz		listNext
		cmp[eax + 8], edx
		jz		done
	listNext :
		mov		ecx, [ecx + 4]
		test	ecx, ecx
		jnz		listIter
		xor eax, eax
	done :
		retn	4
	}
}

InventoryChangesList* InventoryChangesList::Create(UInt32 refID, UInt32 count, ExtendDataList* pExtendDataList)
{
	const auto xData = (InventoryChangesList*)FormHeapAlloc(sizeof(InventoryChangesList));
	if (xData) {
		memset(xData, 0, sizeof(InventoryChangesList));
		xData->AddAt(InventoryChanges::Create(TESForm::GetByID(refID), count, pExtendDataList), eListEnd);
	}
	return xData;
}

void InventoryChangesList::Free(bool bFreeList) {
	UInt32 i = 0;
	InventoryChanges* pX = this->GetNth(i);
	while (pX) {
		pX->Free(bFreeList);
		i++;
		pX = this->GetNth(i);
	}
	FormHeapFree(this);
}

static const UInt32 s_ExtraContainerChangesVtbl = 0x01015BB8;	//	0x0100fb78;
static const UInt32 s_ExtraWornVtbl = 0x01015BDC;
//static const UInt32 s_ExtraWornLeftVtbl							= 0x01015BE8;
static const UInt32 s_ExtraCannotWearVtbl = 0x01015BF4;

static const UInt32 s_ExtraOwnershipVtbl = 0x010158B4;	//	0x0100f874;
static const UInt32 s_ExtraRankVtbl = 0x010158CC;	//	0x0100f88c;
static const UInt32 s_ExtraActionVtbl = 0x01015BAC;
static const UInt32 s_ExtraFactionChangesVtbl = 0x01015F30;
static const UInt32 s_ExtraScriptVtbl = 0X1015914;

//static const UInt32 s_ExtraScript_init							= 0x0042C760;

static const UInt32 s_ExtraHealthVtbl = 0x010158E4;
static const UInt32 s_ExtraLockVtbl = 0x0101589C;
static const UInt32 s_ExtraCountVtbl = 0x010158D8;
static const UInt32 s_ExtraTeleportVtbl = 0x010158A8;
static const UInt32 s_ExtraWeaponModFlagsVtbl = 0x010159A4;

static const UInt32 s_ExtraHotkeyVtbl = 0x0101592C;

static const UInt32 s_ExtraSemaphore = 0x011C3920;
static const UInt32 s_SemaphoreWait = 0x0040FBF0;
static const UInt32 s_SemaphoreLeave = 0x0040FBA0;

static void** g_ExtraSemaphore = (void**)s_ExtraSemaphore;

void* GetExtraSemaphore()
{
	return *g_ExtraSemaphore;
};

void CallSemaphore(void* Semaphore, UInt32 SemaphoreFunc)
{
	_asm pushad
	_asm mov ecx, Semaphore
	_asm call SemaphoreFunc
	_asm popad
};

void CallSemaphore4(void* Semaphore, UInt32 SemaphoreFunc)
{
	_asm pushad
	_asm push ecx;	does not seem to be used at all
	_asm mov ecx, Semaphore
	_asm call SemaphoreFunc
	_asm popad
};


ExtraContainerChanges* ExtraContainerChanges::Create()
{
	const auto xChanges = (ExtraContainerChanges*)BSExtraData::Create(kExtraData_ContainerChanges, sizeof(ExtraContainerChanges), s_ExtraContainerChangesVtbl);
	xChanges->data = nullptr;
	return xChanges;
}

ExtraContainerChanges* ExtraContainerChanges::Create(TESObjectREFR* thisObj, UInt32 refID, UInt32 count, ExtendDataList* pExtendDataList)
{
	const auto xData = (ExtraContainerChanges*)BSExtraData::Create(kExtraData_ContainerChanges, sizeof(ExtraContainerChanges), s_ExtraContainerChangesVtbl);
	if (xData) {
		xData->data = ExtraContainerChangesData::Create(thisObj);
		if (refID) {
			xData->data->objList = InventoryChangesList::Create(refID, count, pExtendDataList);
		}
	}
	return xData;
}


Float64 ExtraContainerChangesData::GetInventoryWeight()
{
	return ThisCall<Float64>(0x4D0900, this, PlayerCharacter::GetSingleton()->isHardcore);
}

void ExtraContainerChangesFree(ExtraContainerChanges* xData, bool bFreeList) {
	if (xData) {
		if (xData->data) {
			if (xData->data->objList && bFreeList) {
				xData->data->objList->Free(true);
			}
			FormHeapFree(xData->data);
		}
		FormHeapFree(xData);
	}
}

void ExtraContainerChangesData::Cleanup()
{
	if (objList) {
		for (const auto iter : *objList) {
			iter->Cleanup();

			// make sure we don't have any NULL ExtraDataList's in extend data, game will choke when saving
			if (iter->extendData == nullptr) continue;

			for (SInt32 index = 0; index < iter->extendData->count(); ) {
				if (const auto xtendData = iter->extendData->GetNth(index); xtendData && !xtendData->m_data) {
					iter->extendData->RemoveNth(index);
					FormHeapFree(xtendData);
				}
				else index++;
			}
		}
	}
}

ExtraDataList* ExtraContainerChangesData::SetEquipped(TESForm* obj, bool bEquipped, bool bForce)
{
	if (true) {
		if (const auto xData = objList->Find(ItemInEntryDataListMatcher(obj))) {
			ExtraDataList* Possible = nullptr;
			if (xData->extendData && (xData->extendData->count() > 0))
				for (const auto iter : *xData->extendData)
					if (iter->HasType(kExtraData_Worn) || iter->HasType(kExtraData_WornLeft)) {
						if (!bEquipped)
							if (bForce || !iter->HasType(kExtraData_WornLeft)) {
								iter->RemoveByType(kExtraData_Worn);
								iter->RemoveByType(kExtraData_WornLeft);
								Cleanup();
								return iter;
							}

					}
					else if (bEquipped && (bForce || !iter->HasType(kExtraData_CannotWear)))
						Possible = iter;
			if (!xData->extendData)
				xData->extendData = ExtendDataList::Create(nullptr);
			if (Possible) {
				Possible->Add(ExtraWorn::Create());
				return Possible;
			}
		}
	}
	return nullptr;
}

bool ExtraContainerChangesData::Remove(TESForm* obj, ExtraDataList* dataList, bool bFree)
{
	for (UInt32 i = 0; i < objList->count(); i++)
		if (objList->GetNth(i)->form == obj) {
			if (const auto found = objList->GetNth(i); dataList && found->extendData) {
				for (UInt32 j = 0; j < found->extendData->count(); j++)
					if (found->extendData->GetNth(j) == dataList)
						found->extendData->RemoveNth(j);
			}
			else if (!dataList && !found->extendData)
				objList->RemoveNth(i);
		}
	return false;
}

ExtraContainerChanges* ExtraContainerChanges::GetForRef(TESObjectREFR* refr)
{
	auto xChanges = reinterpret_cast<ExtraContainerChanges*>(refr->extraDataList.GetByType(kExtraData_ContainerChanges));
	if (!xChanges) {
		xChanges = Create();
		refr->extraDataList.Add(xChanges);
	}
	return xChanges;
}

UInt32 ExtraContainerChangesData::GetAllEquipped(std::vector<InventoryChanges*>& outEntryData)
{
	if (objList) for (const auto entry : *objList)
	{
		if (entry->extendData) for (const auto extraDataList : *entry->extendData)
		{
			if (extraDataList->HasType(kExtraData_Worn)) outEntryData.push_back(entry);
		}
	}
	return outEntryData.size();
}

// static
BSExtraData* BSExtraData::Create(UInt8 xType, UInt32 size, UInt32 vtbl)
{
	void* memory = FormHeapAlloc(size);
	memset(memory, 0, size);
	static_cast<UInt32*>(memory)[0] = vtbl;
	const auto xData = static_cast<BSExtraData*>(memory);
	xData->type = xType;
	return xData;
}

ExtraHealth* ExtraHealth::Create()
{
	const auto xHealth = (ExtraHealth*)BSExtraData::Create(kExtraData_Health, sizeof(ExtraHealth), s_ExtraHealthVtbl);
	return xHealth;
}

ExtraWorn* ExtraWorn::Create()
{
	const auto xWorn = (ExtraWorn*)BSExtraData::Create(kExtraData_Worn, sizeof(ExtraWorn), s_ExtraWornVtbl);
	return xWorn;
}

//ExtraWornLeft* ExtraWornLeft::Create() 
//{
//	ExtraWornLeft* xWornLeft = (ExtraWornLeft*)BSExtraData::Create(kExtraData_WornLeft, sizeof(ExtraWornLeft), s_ExtraWornLeftVtbl);
//	return xWornLeft;
//}

ExtraCannotWear* ExtraCannotWear::Create()
{
	const auto xCannotWear = (ExtraCannotWear*)BSExtraData::Create(kExtraData_CannotWear, sizeof(ExtraCannotWear), s_ExtraCannotWearVtbl);
	return xCannotWear;
}

ExtraLock* ExtraLock::Create()
{
	const auto xLock = (ExtraLock*)BSExtraData::Create(kExtraData_Lock, sizeof(ExtraLock), s_ExtraLockVtbl);
	const auto lockData = (ExtraLockData*)FormHeapAlloc(sizeof(ExtraLockData));
	memset(lockData, 0, sizeof(ExtraLockData));
	xLock->data = lockData;
	return xLock;
}

ExtraCount* ExtraCount::Create(UInt32 count)
{
	const auto xCount = (ExtraCount*)BSExtraData::Create(kExtraData_Count, sizeof(ExtraCount), s_ExtraCountVtbl);
	xCount->count = count;
	return xCount;
}

ExtraTeleport* ExtraTeleport::Create()
{
	const auto tele = (ExtraTeleport*)BSExtraData::Create(kExtraData_Teleport, sizeof(ExtraTeleport), s_ExtraTeleportVtbl);

	// create data
	const auto data = (ExtraTeleportData*)FormHeapAlloc(sizeof(ExtraTeleportData));
	data->linkedDoor = NULL;
	data->yRot = -0.0;
	data->xRot = 0.0;
	data->x = 0.0;
	data->y = 0.0;
	data->z = 0.0;
	data->zRot = 0.0;

	tele->data = data;
	return tele;
}

ExtraWeaponModFlags* ExtraWeaponModFlags::Create()
{
	const auto xWeaponModFlags = (ExtraWeaponModFlags*)BSExtraData::Create(kExtraData_WeaponModFlags, sizeof(ExtraWeaponModFlags), s_ExtraWeaponModFlagsVtbl);

	xWeaponModFlags->flags = 0;

	return xWeaponModFlags;
}

SInt32 GetCountForExtraDataList(ExtraDataList* list)
{
	if (!list)
		return 1;

	const auto xCount = (ExtraCount*)list->GetByType(kExtraData_Count);
	return xCount ? xCount->count : 1;
}

ExtraOwnership* ExtraOwnership::Create()
{
	const auto xOwner = (ExtraOwnership*)BSExtraData::Create(kExtraData_Ownership, sizeof(ExtraOwnership), s_ExtraOwnershipVtbl);
	return xOwner;
}

ExtraRank* ExtraRank::Create()
{
	const auto xRank = (ExtraRank*)BSExtraData::Create(kExtraData_Rank, sizeof(ExtraRank), s_ExtraRankVtbl);
	return xRank;
}

ExtraAction* ExtraAction::Create()
{
	const auto xAction = (ExtraAction*)BSExtraData::Create(kExtraData_Action, sizeof(ExtraAction), s_ExtraActionVtbl);
	return xAction;
}

const char* GetExtraDataName(UInt8 ExtraDataType) {
	switch (ExtraDataType) {
	case	kExtraData_Havok: return "Havok"; break;
	case	kExtraData_Cell3D: return "Cell3D"; break;
	case	kExtraData_CellWaterType: return "CellWaterType"; break;
	case	kExtraData_RegionList: return "RegionList"; break;
	case	kExtraData_SeenData: return "SeenData"; break;
	case	kExtraData_CellMusicType: return "CellMusicType"; break;
	case	kExtraData_CellClimate: return "CellClimate"; break;
	case	kExtraData_ProcessMiddleLow: return "ProcessMiddleLow"; break;
	case	kExtraData_CellCanopyShadowMask: return "CellCanopyShadowMask"; break;
	case	kExtraData_DetachTime: return "DetachTime"; break;
	case	kExtraData_PersistentCell: return "PersistentCell"; break;
	case	kExtraData_Script: return "Script"; break;
	case	kExtraData_Action: return "Action"; break;
	case	kExtraData_StartingPosition: return "StartingPosition"; break;
	case	kExtraData_Anim: return "Anim"; break;
	case	kExtraData_UsedMarkers: return "UsedMarkers"; break;
	case	kExtraData_DistantData: return "DistantData"; break;
	case	kExtraData_RagdollData: return "RagdollData"; break;
	case	kExtraData_ContainerChanges: return "ContainerChanges"; break;
	case	kExtraData_Worn: return "Worn"; break;
	case	kExtraData_WornLeft: return "WornLeft"; break;
	case	kExtraData_PackageStartLocation: return "PackageStartLocation"; break;
	case	kExtraData_Package: return "Package"; break;
	case	kExtraData_TrespassPackage: return "TrespassPackage"; break;
	case	kExtraData_RunOncePacks: return "RunOncePacks"; break;
	case	kExtraData_ReferencePointer: return "ReferencePointer"; break;
	case	kExtraData_Follower: return "Follower"; break;
	case	kExtraData_LevCreaModifier: return "LevCreaModifier"; break;
	case	kExtraData_Ghost: return "Ghost"; break;
	case	kExtraData_OriginalReference: return "OriginalReference"; break;
	case	kExtraData_Ownership: return "Ownership"; break;
	case	kExtraData_Global: return "Global"; break;
	case	kExtraData_Rank: return "Rank"; break;
	case	kExtraData_Count: return "Count"; break;
	case	kExtraData_Health: return "Health"; break;
	case	kExtraData_Uses: return "Uses"; break;
	case	kExtraData_TimeLeft: return "TimeLeft"; break;
	case	kExtraData_Charge: return "Charge"; break;
	case	kExtraData_Light: return "Light"; break;
	case	kExtraData_Lock: return "Lock"; break;
	case	kExtraData_Teleport: return "Teleport"; break;
	case	kExtraData_MapMarker: return "MapMarker"; break;
	case	kExtraData_LeveledCreature: return "LeveledCreature"; break;
	case	kExtraData_LeveledItem: return "LeveledItem"; break;
	case	kExtraData_Scale: return "Scale"; break;
	case	kExtraData_Seed: return "Seed"; break;
	case	kExtraData_PlayerCrimeList: return "PlayerCrimeList"; break;
	case	kExtraData_EnableStateParent: return "EnableStateParent"; break;
	case	kExtraData_EnableStateChildren: return "EnableStateChildren"; break;
	case	kExtraData_ItemDropper: return "ItemDropper"; break;
	case	kExtraData_DroppedItemList: return "DroppedItemList"; break;
	case	kExtraData_RandomTeleportMarker: return "RandomTeleportMarker"; break;
	case	kExtraData_MerchantContainer: return "MerchantContainer"; break;
	case	kExtraData_SavedHavokData: return "SavedHavokData"; break;
	case	kExtraData_CannotWear: return "CannotWear"; break;
	case	kExtraData_Poison: return "Poison"; break;
	case	kExtraData_LastFinishedSequence: return "LastFinishedSequence"; break;
	case	kExtraData_SavedAnimation: return "SavedAnimation"; break;
	case	kExtraData_NorthRotation: return "NorthRotation"; break;
	case	kExtraData_XTarget: return "XTarget"; break;
	case	kExtraData_FriendHits: return "FriendHits"; break;
	case	kExtraData_HeadingTarget: return "HeadingTarget"; break;
	case	kExtraData_RefractionProperty: return "RefractionProperty"; break;
	case	kExtraData_StartingWorldOrCell: return "StartingWorldOrCell"; break;
	case	kExtraData_Hotkey: return "Hotkey"; break;
	case	kExtraData_EditorRefMovedData: return "EditorRefMovedData"; break;
	case	kExtraData_InfoGeneralTopic: return "InfoGeneralTopic"; break;
	case	kExtraData_HasNoRumors: return "HasNoRumors"; break;
	case	kExtraData_Sound: return "Sound"; break;
	case	kExtraData_TerminalState: return "TerminalState"; break;
	case	kExtraData_LinkedRef: return "LinkedRef"; break;
	case	kExtraData_LinkedRefChildren: return "LinkedRefChildren"; break;
	case	kExtraData_ActivateRef: return "ActivateRef"; break;
	case	kExtraData_ActivateRefChildren: return "ActivateRefChildren"; break;
	case	kExtraData_TalkingActor: return "TalkingActor"; break;
	case	kExtraData_ObjectHealth: return "ObjectHealth"; break;
	case	kExtraData_DecalRefs: return "DecalRefs"; break;
	case	kExtraData_CellImageSpace: return "CellImageSpace"; break;
	case	kExtraData_NavMeshPortal: return "NavMeshPortal"; break;
	case	kExtraData_ModelSwap: return "ModelSwap"; break;
	case	kExtraData_Radius: return "Radius"; break;
	case	kExtraData_Radiation: return "Radiation"; break;
	case	kExtraData_FactionChanges: return "FactionChanges"; break;
	case	kExtraData_DismemberedLimbs: return "DismemberedLimbs"; break;
	case	kExtraData_MultiBound: return "MultiBound"; break;
	case	kExtraData_MultiBoundData: return "MultiBoundData"; break;
	case	kExtraData_MultiBoundRef: return "MultiBoundRef"; break;
	case	kExtraData_ReflectedRefs: return "ReflectedRefs"; break;
	case	kExtraData_ReflectorRefs: return "ReflectorRefs"; break;
	case	kExtraData_EmittanceSource: return "EmittanceSource"; break;
	case	kExtraData_RadioData: return "RadioData"; break;
	case	kExtraData_CombatStyle: return "CombatStyle"; break;
	case	kExtraData_Primitive: return "Primitive"; break;
	case	kExtraData_OpenCloseActivateRef: return "OpenCloseActivateRef"; break;
	case	kExtraData_AnimNoteReciever: return "AnimNoteReciever"; break;
	case	kExtraData_Ammo: return "Ammo"; break;
	case	kExtraData_PatrolRefData: return "PatrolRefData"; break;
	case	kExtraData_PackageData: return "PackageData"; break;
	case	kExtraData_OcclusionPlane: return "OcclusionPlane"; break;
	case	kExtraData_CollisionData: return "CollisionData"; break;
	case	kExtraData_SayTopicInfoOnceADay: return "SayTopicInfoOnceADay"; break;
	case	kExtraData_EncounterZone: return "EncounterZone"; break;
	case	kExtraData_SayToTopicInfo: return "SayToTopicInfo"; break;
	case	kExtraData_OcclusionPlaneRefData: return "OcclusionPlaneRefData"; break;
	case	kExtraData_PortalRefData: return "PortalRefData"; break;
	case	kExtraData_Portal: return "Portal"; break;
	case	kExtraData_Room: return "Room"; break;
	case	kExtraData_HealthPerc: return "HealthPerc"; break;
	case	kExtraData_RoomRefData: return "RoomRefData"; break;
	case	kExtraData_GuardedRefData: return "GuardedRefData"; break;
	case	kExtraData_CreatureAwakeSound: return "CreatureAwakeSound"; break;
	case	kExtraData_WaterZoneMap: return "WaterZoneMap"; break;
	case	kExtraData_IgnoredBySandbox: return "IgnoredBySandbox"; break;
	case	kExtraData_CellAcousticSpace: return "CellAcousticSpace"; break;
	case	kExtraData_ReservedMarkers: return "ReservedMarkers"; break;
	case	kExtraData_WeaponIdleSound: return "WeaponIdleSound"; break;
	case	kExtraData_WaterLightRefs: return "WaterLightRefs"; break;
	case	kExtraData_LitWaterRefs: return "LitWaterRefs"; break;
	case	kExtraData_WeaponAttackSound: return "WeaponAttackSound"; break;
	case	kExtraData_ActivateLoopSound: return "ActivateLoopSound"; break;
	case	kExtraData_PatrolRefInUseData: return "PatrolRefInUseData"; break;
	case	kExtraData_AshPileRef: return "AshPileRef"; break;
	case	kExtraData_CreatureMovementSound: return "CreatureMovementSound"; break;
	case	kExtraData_FollowerSwimBreadcrumbs: return "FollowerSwimBreadcrumbs"; break;
	};
	return "unknown";
}

ExtraScript* ExtraScript::Create(TESScriptableForm* pScript, bool create, TESObjectREFR* container) {
	const auto xScript = reinterpret_cast<ExtraScript*>(BSExtraData::Create(kExtraData_Script, sizeof(ExtraScript), s_ExtraScriptVtbl));
	if (xScript && (pScript && pScript->script)) {
		xScript->script = pScript->script;
		if (create) {
			xScript->eventList = xScript->script->CreateEventList();
			if (container)
				xScript->eventList->EventCreate(ScriptEventList::kEvent_OnAdd, container);
		}
	}
	return xScript;
}

ExtraFactionChanges* ExtraFactionChanges::Create()
{
	const auto xFactionChanges = (ExtraFactionChanges*)BSExtraData::Create(kExtraData_FactionChanges, sizeof(ExtraFactionChanges), s_ExtraFactionChangesVtbl);
	const auto FactionChangesData = (FactionListData*)FormHeapAlloc(sizeof(FactionListData));
	memset(FactionChangesData, 0, sizeof(FactionListData));
	xFactionChanges->data = FactionChangesData;
	return xFactionChanges;
}

FactionListData* GetExtraFactionList(BaseExtraList& xDataList)
{
	const auto xFactionChanges = GetByTypeCast(xDataList, FactionChanges);
	if (xFactionChanges)
		return xFactionChanges->data;
	return NULL;
}

SInt8 GetExtraFactionRank(BaseExtraList& xDataList, TESFaction* faction)
{
	const auto xFactionChanges = GetByTypeCast(xDataList, FactionChanges);
	if (xFactionChanges && xFactionChanges->data) {
		ExtraFactionChangesMatcher matcher(faction, xFactionChanges);
		FactionListEntry* pData = xFactionChanges->data->Find(matcher);
		return (pData) ? pData->rank : -1;
	}
	return -1;
}

void SetExtraFactionRank(BaseExtraList& xDataList, TESFaction* faction, SInt8 rank)
{
	FactionListEntry* pData = NULL;
	auto xFactionChanges = GetByTypeCast(xDataList, FactionChanges);
	if (xFactionChanges && xFactionChanges->data) {
		ExtraFactionChangesMatcher matcher(faction, xFactionChanges);
		pData = xFactionChanges->data->Find(matcher);
		if (pData)
			pData->rank = rank;
	}
	if (!pData) {
		if (!xFactionChanges) {
			xFactionChanges = ExtraFactionChanges::Create();
			xDataList.Add(xFactionChanges);
		}
		pData = (FactionListEntry*)FormHeapAlloc(sizeof(FactionListEntry));
		if (pData) {
			pData->faction = faction;
			pData->rank = rank;
			xFactionChanges->data->AddAt(pData, -2);
		}
	}
}

ExtraHotkey* ExtraHotkey::Create()
{
	const auto xHotkey = (ExtraHotkey*)BSExtraData::Create(kExtraData_Hotkey, sizeof(ExtraHotkey), s_ExtraHotkeyVtbl);
	xHotkey->index = 0;
	return xHotkey;
}


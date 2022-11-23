#include <BSExtraData.h>

#include <ExtraData.h>
#include <Objects.h>
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
			ExtraDataList* pExtraDataList = xData->GetNthItem(i);
			while (pExtraDataList)
			{
				for (UInt32 j = 0; j < 0xFF; j++)
					pExtraDataList->RemoveByType(j);
				i++;
				pExtraDataList = xData->GetNthItem(i);
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
		ExtraDataList* pExtraDataList = xData->GetNthItem(i);
		while (pExtraDataList && pExtraDataList != xList) {
			i++;
			pExtraDataList = xData->GetNthItem(i);
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

bool BaseExtraList::IsWorn()
{
	return HasType(kExtraData_Worn);
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
			extendData->RemoveIf(ExtraDataListInExtendDataListMatcher(iter));
	}
	extendData->RemoveIf(ExtraDataListInExtendDataListMatcher(nullptr));
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
		const SInt32 index = extendData->GetIndexOf(ExtraDataListInExtendDataListMatcher(toRemove));
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
	if (reinterpret_cast<ExtraWorn*>(this->GetExtraData(kExtraData_Worn))) return true;
	if (reinterpret_cast<ExtraWornLeft*>(this->GetExtraData(kExtraData_WornLeft))) return true;
	return false;
}

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
	InventoryChanges* pX = this->GetNthItem(i);
	while (pX) {
		pX->Free(bFreeList);
		i++;
		pX = this->GetNthItem(i);
	}
	FormHeapFree(this);
}
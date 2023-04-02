#include <InventoryChanges.h>

#include <Form.h>
#include <BSExtraData.h>
#include <Reference.h>

#include <GameRTTI.h>

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
	const auto xData = static_cast<InventoryChanges*>(GameHeapAlloc(sizeof(InventoryChanges)));
	if (xData) {
		memset(xData, 0, sizeof(InventoryChanges));
		if (pForm) {
			xData->form = pForm;
			xData->countDelta = count;
			if (pExtendDataList == nullptr)
			{
				pExtendDataList = static_cast<ExtendDataList*>(GameHeapAlloc(sizeof(ExtendDataList)));
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
				GameHeapFree(toRemove);
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

Float64 InventoryChanges::GetHealthPercentAlt(bool axonisFix, bool checkDestruction)
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
	else if (checkDestruction)
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

UInt32 InventoryChanges::GetClipSize()
{
	if (form->typeID != kFormType_TESObjectWEAP) return 0;
	return reinterpret_cast<TESObjectWEAP*>(form)->GetClipRounds(HasWeaponMod(0x2));
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


void InventoryChanges::Equip(Actor* actor, ExtraDataList* extra)
{
	actor->EquipItem(form, countDelta, extra ? extra : extendData->first.data, 0, 0, 1);
}

InventoryChanges::WithExtraData InventoryChanges::HotkeyGet(const UInt8 hotkey)
{
	const auto xChanges = reinterpret_cast<ExtraContainerChanges*>(PlayerCharacter::GetSingleton()
		->extraDataList.GetByType(kExtraData_ContainerChanges));
	for (const auto itemIter : *xChanges->data->objList)
	{
		for (const auto iter : *itemIter->extendData)
		{
			const auto xHotKey = reinterpret_cast<ExtraHotkey*>(iter->GetByType(kExtraData_Hotkey));
			if (xHotKey && xHotKey->index == hotkey)
				return { itemIter, iter };
		}
	}
	return {};
}

InventoryChanges::WithExtraData InventoryChanges::HotkeyClear(const UInt8 hotkey)
{
	const auto xChanges = reinterpret_cast<ExtraContainerChanges*>(PlayerCharacter::GetSingleton()
		->extraDataList.GetByType(kExtraData_ContainerChanges));
	for (const auto itemIter : *xChanges->data->objList)
	{
		for (const auto iter : *itemIter->extendData)
		{
			const auto xHotKey = reinterpret_cast<ExtraHotkey*>(iter->GetByType(kExtraData_Hotkey));
			if (xHotKey && xHotKey->index == hotkey) {
				iter->RemoveByType(kExtraData_Hotkey);
				return { itemIter, iter };
			}
		}
	}
	return {};
}

InventoryChanges::WithExtraData InventoryChanges::HotkeySet(const UInt8 hotkey)
{
	const auto cleared = HotkeyClear(hotkey);
	ExtraHotkey* xHotKey = nullptr;
	for (const auto iter : *extendData)
	{
		xHotKey = reinterpret_cast<ExtraHotkey*>(iter->GetByType(kExtraData_Hotkey));
		if (xHotKey) break;
	}
	if (!xHotKey)
	{
		xHotKey = ExtraHotkey::Create();
		if (xHotKey)
		{
			if (!extendData)
				Add(ExtraDataList::Create());
			if (extendData)
			{
				if (!extendData->count())
					extendData->AddAt(ExtraDataList::Create(), 0);
				if (extendData->count())
					extendData->GetNth(0)->Add(xHotKey);
			}
		}
	}
	if (xHotKey) xHotKey->index = hotkey;
	return cleared;
}

ExtraContainerChangesData* ExtraContainerChangesData::Create(TESObjectREFR* owner)
{
	const auto data = static_cast<ExtraContainerChangesData*>(GameHeapAlloc(sizeof(ExtraContainerChangesData)));
	if (data) {
		data->owner = owner;
		data->objList = nullptr;
		data->unk2 = 0.0;
		data->unk3 = 0.0;
	}
	return data;
}

class AcceptTESFormInInventoryChanges
{
	TESForm* m_toMatch;
public:
	AcceptTESFormInInventoryChanges(TESForm* match) : m_toMatch(match) { }
	bool Accept(const InventoryChanges* match) const { return (match && m_toMatch == match->form); }
};

ExtendDataList* ExtraContainerChangesData::Add(TESForm* form, ExtraDataList* dataList)
{
	if (!objList) objList = InventoryChangesList::Create(0, 0, nullptr);

	// try to locate the form
	InventoryChanges* found = objList->Find(AcceptTESFormInInventoryChanges(form));
	if (!found) {
		// add it to the list with a count delta of 0
		found = InventoryChanges::Create(form, 0);
		objList->AddAt(found, eListEnd);
	}

	return found->Add(dataList);
}

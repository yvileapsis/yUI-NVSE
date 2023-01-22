#include <InventoryChanges.h>

#include <Form.h>
#include <BSExtraData.h>
#include <Reference.h>

InventoryChanges* InventoryChanges::HotkeyGet(const UInt8 hotkey)
{
	const auto xChanges = reinterpret_cast<ExtraContainerChanges*>(PlayerCharacter::GetSingleton()
		->extraDataList.GetByType(kExtraData_ContainerChanges));
	for (const auto itemIter : *xChanges->data->objList)
	{
		for (const auto iter : *itemIter->extendData)
		{
			const auto xHotKey = reinterpret_cast<ExtraHotkey*>(iter->GetByType(kExtraData_Hotkey));
			if (xHotKey && xHotKey->index == hotkey) return itemIter;
		}
	}
	return nullptr;
}

InventoryChanges* InventoryChanges::HotkeyClear(const UInt8 hotkey)
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
				return itemIter;
			}
		}
	}
	return nullptr;
}

InventoryChanges* InventoryChanges::HotkeySet(const UInt8 hotkey)
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
	const auto data = static_cast<ExtraContainerChangesData*>(FormHeapAlloc(sizeof(ExtraContainerChangesData)));
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

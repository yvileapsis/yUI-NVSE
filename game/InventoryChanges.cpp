#include <InventoryChanges.h>

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

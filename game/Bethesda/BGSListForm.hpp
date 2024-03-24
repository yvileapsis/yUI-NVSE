#pragma once

#include "TESForm.hpp"

class BGSListForm : public TESForm {
public:
	BGSListForm();
	~BGSListForm();

	BSSimpleList<TESForm*>	list;
	UInt32					numAddedObjects;
#if 0
	UInt32 Count() const {
		return list.ItemsInList();
	}

	TESForm* GetNthForm(SInt32 n) const {
		auto result = list.GetAt(n);
		if (result)
			return result->GetItem();
		return nullptr;
	}

	SInt32 AddAt(TESForm* pForm, SInt32 n, bool const checkDupes = false) {
		if (checkDupes) {
			if (GetIndexOf(pForm) != -1)
				return -1;
		}
		auto const result = list.AddAt(n, pForm);
		if (result >= 0 && IsAddedObject(n))
			numAddedObjects++;

		return result;
	}


	SInt32 GetIndexOf(TESForm* pForm);

	TESForm* RemoveNthForm(SInt32 n) {
		auto entry = list.RemoveAt(n);
		TESForm* form = entry ? entry->GetItem() : nullptr;

		if (form && IsAddedObject(n)) {
			if (numAddedObjects == 0)
			{
				_MESSAGE("BGSListForm::RemoveNthForm: numAddedObjects = 0");
			}
			else
			{
				numAddedObjects--;
			}
		}

		return form;
	}

	TESForm* ReplaceNthForm(SInt32 n, TESForm* pReplaceWith) {
		auto entry = list.ReplaceAt(n, pReplaceWith);
		return entry ? entry->GetItem() : nullptr;;
	}

	SInt32 RemoveForm(TESForm* pForm);
	SInt32 ReplaceForm(TESForm* pForm, TESForm* pReplaceWith);

	bool IsAddedObject(SInt32 idx) {
		return (idx >= 0) && (idx < numAddedObjects);
	}

#if RUNTIME
	[[nodiscard]] static game_unique_ptr<BGSListForm> MakeUnique();
#endif
#endif
};

ASSERT_SIZE(BGSListForm, 0x024);
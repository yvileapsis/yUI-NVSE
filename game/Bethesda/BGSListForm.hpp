#pragma once
#include "TESForm.hpp"

// 0x24
class BGSListForm :
	public TESForm,					// 00
	public BSSimpleList<TESForm*>	// 18 Added as a parent to be able to use BGSList as list
{
public:
	BGSListForm();
	~BGSListForm();

	UInt32	uiNumAddedObjects;	// number of objects added via script - assumed to be at the start of the list

	SInt32 AddAt(TESForm* pForm, SInt32 n, bool const checkDupes = false) {
		if (checkDupes) {
			if (GetIndexOf(pForm) != eListInvalid)
				return eListInvalid;
		}
		auto const result = BSSimpleList::AddAt(pForm, n);
		if (result >= 0 && IsAddedObject(n))
			uiNumAddedObjects++;

		return result;
	}

	SInt32 GetIndexOf(TESForm* pForm);


	SInt32 RemoveForm(TESForm* pForm);
	SInt32 ReplaceForm(TESForm* pForm, TESForm* pReplaceWith);

	bool IsAddedObject(SInt32 idx) { return (idx >= 0) && (idx < uiNumAddedObjects); }

#if RUNTIME
	[[nodiscard]] static game_unique_ptr<BGSListForm> MakeUnique();
#endif

	SInt32 AddAt(TESForm* pForm, const SInt32 n) {
		const SInt32 result = BSSimpleList::AddAt(pForm, n);
		if (result >= 0 && IsAddedObject(n)) uiNumAddedObjects++;
		return result;
	}

	SInt32 GetIndexOf(TESForm* pForm);

	TESForm* RemoveNth(SInt32 n) {
		TESForm* form = BSSimpleList::RemoveNth(n);
		if (form && IsAddedObject(n)) uiNumAddedObjects--;
		return form;
	}

	SInt32 RemoveForm(TESForm* pForm);
	SInt32 ReplaceForm(TESForm* pForm, TESForm* pReplaceWith);

	bool IsAddedObject(SInt32 idx) { return (idx >= 0) && (idx < uiNumAddedObjects); }
	bool ContainsRecursive(TESForm* form, UInt32 reclvl = 0);
};
static_assert(sizeof(BGSListForm) == 0x24);
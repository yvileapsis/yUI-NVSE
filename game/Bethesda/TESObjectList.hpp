#pragma once
#include "TESObject.hpp"

class TESObjectList {
public:
	UInt32		uiCount;
	TESObject*	pHead;
	TESObject*	pTail;
	UInt32		unkC;

	inline UInt32		GetSize() const;
	inline TESObject*	GetHead() const;
	inline TESObject*	GetTail() const;

	TESObject* GetAt(UInt32 index) const;

	template <typename Function>
	void ForEach(Function&& fn) const {
		TESObject* pCurrent = GetHead();
		while (pCurrent) {
			fn(pCurrent);
			pCurrent = pCurrent->GetNext();
		}
	}
};

ASSERT_SIZE(TESObjectList, 0x10);
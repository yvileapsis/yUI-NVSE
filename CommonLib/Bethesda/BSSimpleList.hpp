#pragma once

#include "BSMemObject.hpp"


template <class T_Data>
class BSSimpleList : public BSMemObject {
public:
	BSSimpleList() : m_item(0), m_pkNext(0) { }
	BSSimpleList(T_Data& item) : m_item(item), m_pkNext(0) {}
	BSSimpleList(BSSimpleList<T_Data>& entry) : m_item(entry.m_item), m_pkNext(0) {}
	~BSSimpleList() { RemoveAll(); }

	T_Data					m_item;
	BSSimpleList<T_Data>*	m_pkNext;

	__forceinline const T_Data GetItem() const { return m_item; }
	__forceinline T_Data GetItem() { return m_item; }
	__forceinline void SetItem(T_Data aItem) { m_item = aItem; }

	__forceinline const BSSimpleList<T_Data>* GetNext() const { return m_pkNext; }
	__forceinline BSSimpleList<T_Data>* GetNext() { return m_pkNext; }
	__forceinline void SetNext(BSSimpleList<T_Data>* aNext) { m_pkNext = aNext; }

	// 0x8256D0
	bool IsEmpty() const { return !m_pkNext && !m_item; }

	// Custom
	const BSSimpleList<T_Data>* GetHead() const { return this; }
	BSSimpleList<T_Data>* GetHead() { return this; }

	// Custom
	BSSimpleList<T_Data>* GetTail() const {
		BSSimpleList<T_Data>* kIter;
		for (kIter = (BSSimpleList<T_Data>*)this; kIter->GetNext(); kIter = kIter->GetNext()) {}
		return kIter;
	}

#if 1
	class Iterator {
	public:
		Iterator(BSSimpleList<T_Data>* node) : m_node(node) {}

		T_Data& operator*() { return m_node->m_item; }
		const T_Data& operator*() const { return m_node->m_item; }

		Iterator& operator++() {
			if (m_node)
				m_node = m_node->m_pkNext;
			return *this;
		}

		bool operator!=(const Iterator& other) const {
			return m_node != other.m_node;
		}

	private:
		BSSimpleList<T_Data>* m_node;
	};

	Iterator begin() { return Iterator(this); }
	Iterator end() { return Iterator(nullptr); }
#endif

	// 0x631540
	void AddHead(T_Data& aItem) {
		if (!aItem)
			return;

		BSSimpleList<T_Data>* pHead = GetHead();

		if (pHead->GetItem()) {
			BSSimpleList<T_Data>* pNewEntry = new BSSimpleList<T_Data>(aItem);
			pNewEntry->SetNext(pHead);
			SetNext(pNewEntry);
			pHead->SetItem(aItem);
		}
		else {
			pHead->SetItem(aItem);
		}
	}

	// 0xAF25B0, 0x905820, 0xB63BF0
	void AddTail(T_Data& aItem) {
		if (!aItem)
			return;

		BSSimpleList<T_Data>* pTail = GetTail();

		if (pTail->GetItem()) {
			BSSimpleList<T_Data>* pkNewNode = new BSSimpleList<T_Data>(aItem);
			pTail->SetNext(pkNewNode);
		}
		else {
			pTail->SetItem(aItem);
		}
	}

	// 0x5F65D0
	__forceinline bool IsInList(T_Data& aItem) const {
		BSSimpleList<T_Data>* iter = (BSSimpleList<T_Data>*)this;
		while (iter && m_item != aItem)
			iter = iter->GetNext();
		return iter != 0;
	}

	__forceinline BSSimpleList<T_Data>* GetPos(T_Data& aItem) const {
		BSSimpleList<T_Data>* iter = (BSSimpleList<T_Data>*)this;
		while (iter && m_item != aItem)
			iter = iter->GetNext();
		return iter;
	}

	// 0x5AE380
	__forceinline UInt32 ItemsInList() const {
		UInt32 uiCount = 0;
		BSSimpleList<T_Data>* iter = (BSSimpleList<T_Data>*)this;
		while (iter) {
			if (iter->m_item)
				++uiCount;

			iter = iter->GetNext();
		}
		return uiCount;
	}

	// 0xB64EC0
	void RemoveAll() {
		if (m_pkNext) {
			BSSimpleList<T_Data>* pNext = 0;
			do {
				auto pCurrNode = GetNext();
				pNext = pCurrNode->GetNext();
				pCurrNode->SetNext(0);
				delete m_pkNext;
				SetNext(pNext);
			} while (pNext);
		}
		m_item = 0;
	}

	// 0xB99730
	void RemoveHead() {
		auto pNext = GetNext();
		if (pNext) {
			m_pkNext = pNext->m_pkNext;
			m_item = pNext->m_item;
			pNext->m_pkNext = nullptr;
			delete pNext;
		}
		else {
			m_item = 0;
		}
	}

	void RemoveTail() {
		BSSimpleList<T_Data>* pTail = GetTail();
		if (pTail) {
			BSSimpleList<T_Data>* pIter = this;
			while (pIter->GetNext() != pTail)
				pIter = pIter->GetNext();

			pIter->SetNext(0);
			delete pTail;
		}
		else {
			m_item = 0;
		}
	}

	// 0x905330
	void Remove(T_Data& aItem) {
		if (!aItem || IsEmpty())
			return;

		BSSimpleList<T_Data>* pIter = this;
		BSSimpleList<T_Data>* pPrev = this;

		while (pIter && pIter->m_item != aItem) {
			pPrev = pIter;
			pIter = pIter->m_pkNext;
		}

		if (!pIter)
			return;

		if (pIter == this) {
			if (GetNext()) {
				auto pNext = m_pkNext;
				SetNext(pNext->m_pkNext);
				m_item = pNext->GetItem();
				pNext->m_pkNext = nullptr;
				if (pNext)
					delete pNext;
			}
			else {
				m_item = 0;
			}
		}
		else {
			pPrev->SetNext(pIter->GetNext());
			pIter->SetNext(0);
			if (pIter)
				delete pIter;
		}
	}

	// Custom methods

	BSSimpleList<T_Data>* GetAt(UInt32 auiIndex) const {
		UInt32 i = 0;
		BSSimpleList<T_Data>* pIter = (BSSimpleList<T_Data>*)this;
		while (pIter) {
			if (i == auiIndex)
				return pIter;

			pIter = pIter->GetNext();
			++i;
		}
		return nullptr;
	}

	SInt32 GetIndexOf(T_Data& aItem) const {
		SInt32 iIndex = 0;
		BSSimpleList<T_Data>* pIter = (BSSimpleList<T_Data>*)this;
		while (pIter) {
			if (pIter->m_item == aItem)
				return iIndex;

			pIter = pIter->GetNext();
			++iIndex;
		}
		return -1;
	}

	template <typename FUNC>
	SInt32 GetIndexOf(FUNC&& func) const {
		SInt32 iIndex = 0;
		BSSimpleList<T_Data>* pIter = (BSSimpleList<T_Data>*)this;
		while (pIter) {
			if (func(pIter))
				return iIndex;

			pIter = pIter->GetNext();
			++iIndex;
		}
		return -1;
	}

	BSSimpleList<T_Data>* Find(T_Data& aItem) const {
		BSSimpleList<T_Data>* iter = (BSSimpleList<T_Data>*)this;
		while (iter) {
			if (iter->m_item == aItem)
				return iter;

			iter = iter->GetNext();
		}
		return nullptr;
	}

	template <typename FUNC>
	BSSimpleList<T_Data>* Find(FUNC&& func) const {
		BSSimpleList<T_Data>* iter = (BSSimpleList<T_Data>*)this;
		while (iter) {
			if (func(iter))
				return iter;

			iter = iter->GetNext();
		}
		return nullptr;
	}

	template <typename FUNC>
	bool IsInList(FUNC&& func) const {
		return Find(func) != nullptr;
	}

	template <typename FUNC>
	void ForEach(FUNC&& func) {
		BSSimpleList<T_Data>* iter = (BSSimpleList<T_Data>*)this;
		while (iter) {
			func(iter);
			iter = iter->GetNext();
		}
	}

	BSSimpleList<T_Data>* ReplaceAt(UInt32 auiIndex, T_Data& aItem) {
		BSSimpleList<T_Data>* pIter = GetAt(auiIndex);
		BSSimpleList<T_Data>* pReplaced = nullptr;
		if (pIter) {
			pReplaced = new BSSimpleList<T_Data>(pIter->GetItem());
			pIter->SetItem(aItem);
		}
		return pReplaced;
	}

	BSSimpleList<T_Data>* SetAt(UInt32 auiIndex, T_Data& aItem) {
		BSSimpleList<T_Data>* pIter = GetAt(auiIndex);
		if (pIter)
			pIter->SetItem(aItem);
		else
			AddTail(aItem);
		return pIter;
	}

	SInt32 AddAt(UInt32 auiIndex, T_Data& aItem) {
		BSSimpleList<T_Data>* pIter = GetAt(auiIndex);
		if (pIter) {
			if (pIter->GetItem()) {
				BSSimpleList<T_Data>* pkNewNode = new BSSimpleList<T_Data>(aItem);
				pkNewNode->SetNext(pIter->GetNext());
				pIter->SetNext(pkNewNode);
			}
			else {
				pIter->SetItem(aItem);
			}
		}
		else {
			AddTail(aItem);
		}
		return pIter;
	}

	BSSimpleList<T_Data>* RemoveAt(UInt32 auiIndex) {
		BSSimpleList<T_Data>* pIter = GetAt(auiIndex);
		if (pIter)
			Remove(pIter->GetItem());
		return pIter;
	}

	T_Data TailItem() const {
		const auto tail = GetTail();
		return (tail) ? tail->m_item : nullptr;
	}
};

ASSERT_SIZE(BSSimpleList<UInt32>, 0x8);
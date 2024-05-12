#pragma once

#include "NiTListBase.hpp"

template <class Allocator, class T_Data>
class NiTPointerListBase : public NiTListBase <Allocator, T_Data> {
public:
	~NiTPointerListBase() { RemoveAll(); };

	// 0xB71300
	void RemoveAll() {
		NiTListItem<T_Data>* pkCurr = NiTListBase<Allocator, T_Data>::m_pkHead;
		while (pkCurr)
		{
			NiTListItem<T_Data>* pkDel = pkCurr;
			pkCurr = pkCurr->m_pkNext;
			DeleteItem(pkDel);
		}

		NiTListBase<Allocator, T_Data>::m_kAllocator.m_uiCount = 0;
		NiTListBase<Allocator, T_Data>::m_pkHead = 0;
		NiTListBase<Allocator, T_Data>::m_pkTail = 0;
	};

	// 0x76B660
	void AddHead(const T_Data& element) {
		NiTListItem<T_Data>* pkNode;
		pkNode = (NiTListItem<T_Data>*) NewItem();

		pkNode->m_element = element;

		NiTListBase<Allocator, T_Data>::AddNodeHead(pkNode);
	}

	// 0xB58570
	void AddTail(const T_Data& element) {
		NiTListItem<T_Data>* pkNode;
		pkNode = (NiTListItem<T_Data>*) NewItem();

		pkNode->m_element = element;

		NiTListBase<Allocator, T_Data>::AddNodeTail(pkNode);
	}

	// 0xB713C0
	NiTListIterator InsertBefore(NiTListIterator kPos, const T_Data& element) {
		NiTListItem<T_Data>* pkNode;
		pkNode = (NiTListItem<T_Data>*) NewItem();

		pkNode->m_element = element;

		return NiTListBase<Allocator, T_Data>::AddNodeBefore(kPos, pkNode);
	};
	NiTListIterator InsertAfter(NiTListIterator kPos, const T_Data& element) {
		NiTListItem<T_Data>* pkNode;
		pkNode = (NiTListItem<T_Data>*) NewItem();

		pkNode->m_element = element;

		return NiTListBase<Allocator, T_Data>::AddNodeAfter(kPos, pkNode);
	};

	const T_Data& GetHead() const { return NiTListBase<Allocator, T_Data>::m_pkHead->m_element; };
	const T_Data& GetTail() const { return NiTListBase<Allocator, T_Data>::m_pkTail->m_element; };
	// 0x6317A0
	const T_Data& Get(NiTListIterator kPos) const { return ((NiTListItem<T_Data>*) kPos)->m_element; };

	const T_Data& GetNext(NiTListIterator& kPos) const {
		const T_Data& element = ((NiTListItem<T_Data>*) kPos)->m_element;

		kPos = ((NiTListItem<T_Data>*) kPos)->m_pkNext;
		return element;
	};
	const T_Data& GetPrev(NiTListIterator& kPos) const {
		const T_Data& element = ((NiTListItem<T_Data>*) kPos)->m_element;
		kPos = ((NiTListItem<T_Data>*) kPos)->m_pkPrev;

		return element;
	};

	// 0xB602D0
	T_Data RemoveHead() {
		NiTListItem<T_Data>* pkNode = NiTListBase<Allocator, T_Data>::m_pkHead;

		NiTListBase<Allocator, T_Data>::m_pkHead = NiTListBase<Allocator, T_Data>::m_pkHead->m_pkNext;

		if (NiTListBase<Allocator, T_Data>::m_pkHead)
			NiTListBase<Allocator, T_Data>::m_pkHead->m_pkPrev = 0;
		else
			NiTListBase<Allocator, T_Data>::m_pkTail = 0;

		T_Data element = pkNode->m_element;

		DeleteItem(pkNode);

		NiTListBase<Allocator, T_Data>::m_kAllocator.m_uiCount--;

		return element;
	};

	// 0xE7D5B0
	T_Data RemoveTail() {
		NiTListItem<T_Data>* pkNode = NiTListBase<Allocator, T_Data>::m_pkTail;

		NiTListBase<Allocator, T_Data>::m_pkTail = NiTListBase<Allocator,
			T_Data>::m_pkTail->m_pkPrev;

		if (NiTListBase<Allocator, T_Data>::m_pkTail)
			NiTListBase<Allocator, T_Data>::m_pkTail->m_pkNext = 0;
		else
			NiTListBase<Allocator, T_Data>::m_pkHead = 0;

		T_Data element = pkNode->m_element;

		DeleteItem(pkNode);

		NiTListBase<Allocator, T_Data>::m_kAllocator.m_uiCount--;

		return element;
	};

	// 0xE7DB70
	T_Data Remove(const T_Data& element) {
		NiTListIterator kPos = NiTPointerListBase<Allocator, T_Data>::FindPos(element);

		return kPos ? RemovePos(kPos) : element;
	};

	// 0xE7D7F0
	T_Data RemovePos(NiTListIterator& kPos) {
		NiTListItem<T_Data>* pkNode = (NiTListItem<T_Data>*) kPos;

		if (pkNode == NiTListBase<Allocator, T_Data>::m_pkHead) {
			kPos = pkNode->m_pkNext; // kPos points to new head
			return RemoveHead();
		}
		if (pkNode == NiTListBase<Allocator, T_Data>::m_pkTail) {
			kPos = 0; // kPos has walked off end of list
			return RemoveTail();
		}

		NiTListItem<T_Data>* pkPrev = pkNode->m_pkPrev;
		NiTListItem<T_Data>* pkNext = pkNode->m_pkNext;

		kPos = pkNext;

		if (pkPrev)
			pkPrev->m_pkNext = pkNext;
		if (pkNext)
			pkNext->m_pkPrev = pkPrev;

		T_Data element = pkNode->m_element;

		DeleteItem(pkNode);

		NiTListBase<Allocator, T_Data>::m_kAllocator.m_uiCount--;

		return element;
	}

	// 0xE7DB70
	NiTListIterator FindPos(const T_Data& element, NiTListIterator kStart = 0) const {
		if (kStart == 0)
			kStart = NiTPointerListBase<Allocator, T_Data>::GetHeadPos();

		while (kStart) {
			NiTListIterator kPos = kStart;
			if (element == GetNext(kStart))
				return kPos;
		}
		return 0;
	};

	template <typename FUNC>
	void ForEach(FUNC func) {
		UInt32 i = 0;
		NiTListIterator kPos = NiTPointerListBase<Allocator, T_Data>::GetHeadPos();
		while (kPos) {
			func(GetNext(kPos), i++);
		}
	};

protected:
	// 0x49FA80
	NiTListItem<T_Data>* NewItem() {
		return (NiTListItem<T_Data>*)NiTListBase<Allocator, T_Data>::m_kAllocator.Allocate();
	};
	// 0xE6E8A0
	void DeleteItem(NiTListItem<T_Data>* pkItem) {
		pkItem->m_element = 0;
		NiTListBase<Allocator, T_Data>::m_kAllocator.Deallocate(pkItem);
	}
};
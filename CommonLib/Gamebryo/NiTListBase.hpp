#pragma once

#include "NiMemObject.hpp"

typedef void* NiTListIterator;

template <class T_Data>
class NiTListItem : public NiMemObject {
public:
	NiTListItem*	m_pkNext;
	NiTListItem*	m_pkPrev;
	T_Data		  m_element;
};

template <class Allocator, class T_Data>
class  NiTListBase : public NiMemObject {
public:
	NiTListBase() : m_pkHead(0), m_pkTail(0) { m_kAllocator.m_uiCount = 0; };
	~NiTListBase() {};

	NiTListBase(const NiTListBase&) = delete;
	NiTListBase& operator=(const NiTListBase&) = delete;

	struct AntiBloatAllocator : public Allocator {
		UInt32 m_uiCount;
	};

	NiTListItem<T_Data>* m_pkHead;
	NiTListItem<T_Data>* m_pkTail;
	AntiBloatAllocator   m_kAllocator;

	inline UInt32 GetSize() const { return m_kAllocator.m_uiCount; };
	inline bool IsEmpty() const { return m_kAllocator.m_uiCount == 0; };

	inline NiTListIterator GetHeadPos() const { return m_pkHead; };
	inline NiTListIterator GetTailPos() const { return m_pkTail; };

	// 0x7B52D0
	inline NiTListIterator GetNextPos(NiTListIterator kPos) const { return kPos ? ((NiTListItem<T_Data>*) kPos)->m_pkNext : 0; };
	inline NiTListIterator GetPrevPos(NiTListIterator kPos) const { return kPos ? ((NiTListItem<T_Data>*) kPos)->m_pkPrev : 0; };

	// 0x49FA20
	void AddNodeHead(NiTListItem<T_Data>* pkNode) {
		pkNode->m_pkPrev = 0;
		pkNode->m_pkNext = m_pkHead;

		if (m_pkHead)
			m_pkHead->m_pkPrev = pkNode;
		else
			m_pkTail = pkNode;

		m_pkHead = pkNode;
		m_kAllocator.m_uiCount++;
	};

	// 0xB5A9A0
	void AddNodeTail(NiTListItem<T_Data>* pkNode) {
		pkNode->m_pkNext = 0;
		pkNode->m_pkPrev = m_pkTail;

		if (m_pkTail)
			m_pkTail->m_pkNext = pkNode;
		else
			m_pkHead = pkNode;

		m_pkTail = pkNode;
		m_kAllocator.m_uiCount++;
	};

	NiTListIterator AddNodeAfter(NiTListIterator kPos, NiTListItem<T_Data>* pkNode) {
		NiTListItem<T_Data>* pkPrev = (NiTListItem<T_Data>*) kPos;

		pkNode->m_pkPrev = pkPrev;
		pkNode->m_pkNext = pkPrev->m_pkNext;

		if (pkPrev->m_pkNext)
			pkPrev->m_pkNext->m_pkPrev = pkNode;
		else
			m_pkTail = pkNode;

		pkPrev->m_pkNext = pkNode;

		m_kAllocator.m_uiCount++;
		return pkNode;
	};

	NiTListIterator AddNodeBefore(NiTListIterator kPos, NiTListItem<T_Data>* pkNode) {
		NiTListItem<T_Data>* pkNext = (NiTListItem<T_Data>*) kPos;

		pkNode->m_pkNext = pkNext;
		pkNode->m_pkPrev = pkNext->m_pkPrev;

		if (pkNext->m_pkPrev)
			pkNext->m_pkPrev->m_pkNext = pkNode;
		else
			m_pkHead = pkNode;

		pkNext->m_pkPrev = pkNode;

		m_kAllocator.m_uiCount++;
		return pkNode;
	};

	void MoveToBefore(NiTListIterator kObjectToMove, NiTListIterator kDestination) {
		NiTListItem<T_Data>* pToMove = (NiTListItem<T_Data>*) kObjectToMove;
		NiTListItem<T_Data>* pDestination = (NiTListItem<T_Data>*) kDestination;

		if (pToMove != pDestination) {
			if (m_pkHead == pToMove)
				m_pkHead = pToMove->m_pkNext;
			if (m_pkHead == pDestination)
				m_pkHead = pToMove;
			if (m_pkTail == pToMove)
				m_pkTail = pToMove->m_pkPrev;

			if (pToMove->m_pkNext)
				pToMove->m_pkNext->m_pkPrev = pToMove->m_pkPrev;

			if (pToMove->m_pkPrev)
				pToMove->m_pkPrev->m_pkNext = pToMove->m_pkNext;

			pToMove->m_pkPrev = pDestination->m_pkPrev;
			pToMove->m_pkNext = pDestination;

			if (pDestination->m_pkPrev)
				pDestination->m_pkPrev->m_pkNext = pToMove;

			pDestination->m_pkPrev = pToMove;
		}
	};

	class Iterator
	{
		NiTListItem<T_Data>* m_curr;

	public:
		Iterator	operator++()
		{
			m_curr = m_curr->m_pkNext;
			return *this;
		}
		T_Data operator->() const { return m_curr->m_element; }
		T_Data& operator*() const { return m_curr->m_element; }
		Iterator& operator=(const Iterator& rhs)
		{
			m_curr = rhs.m_curr;
			return *this;
		}
		bool		operator!=(const Iterator& rhs) { return m_curr && m_curr->m_element; }

		Iterator() : m_curr(nullptr) {}
		Iterator(NiTListItem<T_Data>* node) : m_curr(node) {}
	};

	Iterator begin() const { return Iterator(m_pkHead); }
	Iterator end() const { return Iterator(); }
};
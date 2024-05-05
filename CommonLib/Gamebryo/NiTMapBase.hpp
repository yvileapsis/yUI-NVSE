#pragma once

#include "NiMemObject.hpp"

typedef void* NiTMapIterator;

template <class T_Key, class T_Data> class NiTMapItem : public NiMemObject {
public:
	NiTMapItem* m_pkNext;
	T_Key	   m_key;
	T_Data	  m_val;
};

template <class Allocator, class T_Key, class T_Data> class NiTMapBase : public NiMemObject {
public:
	typedef NiTMapItem<T_Key, T_Data> Entry;

	struct AntiBloatAllocator : public Allocator {
		UInt32 m_uiCount;
	};

	NiTMapBase(UInt32 uiHashSize = 37) {
		m_uiHashSize = uiHashSize;
		m_kAllocator.m_uiCount = 0;

		UInt32 uiSize = sizeof(NiTMapItem<T_Key, T_Data>*) * m_uiHashSize;
		m_ppkHashTable = (NiTMapItem<T_Key, T_Data>**)NiAlloc(uiSize);
		memset(m_ppkHashTable, 0, m_uiHashSize * sizeof(NiTMapItem<T_Key, T_Data>*));
	};
	virtual							 ~NiTMapBase();
	virtual UInt32					  KeyToHashIndex(T_Key key);
	virtual bool						IsKeysEqual(T_Key key1, T_Key key2);
	virtual void						SetValue(NiTMapItem<T_Key, T_Data>* apItem, T_Key key, T_Data data);
	virtual void						ClearValue(NiTMapItem<T_Key, T_Data>* apItem);
	virtual NiTMapItem<T_Key, T_Data>*  NewItem() = 0;
	virtual void						DeleteItem(NiTMapItem<T_Key, T_Data>* apItem) = 0;

	UInt32					  m_uiHashSize;
	NiTMapItem<T_Key, T_Data>** m_ppkHashTable;
	AntiBloatAllocator		  m_kAllocator;

	UInt32 GetCount() const { return m_kAllocator.m_uiCount; }

	bool GetAt(T_Key key, T_Data& dataOut) {
		UInt32 uiHashIndex = KeyToHashIndex(key);
		NiTMapItem<T_Key, T_Data>* pItem = m_ppkHashTable[uiHashIndex];
		while (pItem) {
			if (IsKeysEqual(pItem->m_key, key)) {
				dataOut = pItem->m_val;
				return true;
			}
			pItem = pItem->m_pkNext;
		}
		return false;
	}

	void SetAt(T_Key key, T_Data data) {
		UInt32 uiHashIndex = KeyToHashIndex(key);
		NiTMapItem<T_Key, T_Data>* pItem = m_ppkHashTable[uiHashIndex];
		while (pItem) {
			if (IsKeysEqual(pItem->m_key, key)) {
				pItem->m_val = data;
				return;
			}
			pItem = pItem->m_pkNext;
		}
		NiTMapItem<T_Key, T_Data>* pNewEntry = NewItem();
		SetValue(pNewEntry, key, data);
		pNewEntry->m_pkNext = m_ppkHashTable[uiHashIndex];
		m_ppkHashTable[uiHashIndex] = pNewEntry;
		m_kAllocator.m_uiCount++;
	}

	T_Data* Lookup(T_Key key) {
		T_Data* dataOut = nullptr;
		GetAt(key, *dataOut);
		return dataOut;
	}

	bool Insert(NiTMapItem<T_Key, T_Data>* apNewEntry) {
		// game code does not appear to care about ordering of entries in buckets
		UInt32 bucket = apNewEntry->m_key % m_uiHashSize;
		NiTMapItem<T_Key, T_Data>* prev = NULL;
		for (NiTMapItem<T_Key, T_Data>* cur = m_ppkHashTable[bucket]; cur; cur = cur->m_pkNext) {
			if (cur->m_key == apNewEntry->m_key) {
				return false;
			}
			else if (!cur->m_pkNext) {
				prev = cur;
				break;
			}
		}

		if (prev) {
			prev->m_pkNext = apNewEntry;
		}
		else {
			m_ppkHashTable[bucket] = apNewEntry;
		}

		m_uiHashSize++;
		return true;
	}

	// 0x405430
	bool RemoveAt(T_Key key) {
		UInt32 uiIndex = KeyToHashIndex(key);
		NiTMapItem<T_Key, T_Data>* pItem = m_ppkHashTable[uiIndex];

		if (!pItem)
			return false;

		if (IsKeysEqual(key, pItem->m_key)) {
			m_ppkHashTable[uiIndex] = pItem->m_pkNext;
			ClearValue(pItem);
			DeleteItem(pItem);
			--m_kAllocator.m_uiCount;
			return true;
		}

		NiTMapItem<T_Key, T_Data>* pPrev = pItem;
		NiTMapItem<T_Key, T_Data>* i = pItem->m_pkNext;

		for (i; i && !IsKeysEqual(key, i->m_key); i = i->m_pkNext)
			pPrev = i;

		if (!i)
			return false;

		pPrev->m_pkNext = i->m_pkNext;
		ClearValue(i);
		DeleteItem(i);
		--m_kAllocator.m_uiCount;
		return true;
	}

	void RemoveAll() {
		for (UInt32 i = 0; i < m_uiHashSize; i++) {
			while (m_ppkHashTable[i]) {
				NiTMapItem<T_Key, T_Data>* pItem = m_ppkHashTable[i];
				m_ppkHashTable[i] = m_ppkHashTable[i]->m_pkNext;
				ClearValue(pItem);
				DeleteItem(pItem);
			}
		}

		m_kAllocator.m_uiCount = 0;
	}

	NiTMapIterator GetFirstPos() const {
		for (UInt32 i = 0; i < m_uiHashSize; i++) {
			if (m_ppkHashTable[i])
				return m_ppkHashTable[i];
		}
		return 0;
	}

	void GetNext(NiTMapIterator& pos, T_Key& key, T_Data& val) {
		NiTMapItem<T_Key, T_Data>* pItem = (NiTMapItem<T_Key, T_Data>*) pos;

		key = pItem->m_key;
		val = pItem->m_val;

		if (pItem->m_pkNext) {
			pos = pItem->m_pkNext;
			return;
		}

		UInt32 i = KeyToHashIndex(pItem->m_key);
		for (++i; i < m_uiHashSize; i++)
		{
			pItem = m_ppkHashTable[i];
			if (pItem)
			{
				pos = pItem;
				return;
			}
		}

		pos = 0;
	}


	class Iterator
	{
		friend NiTMapBase;

		NiTMapBase* m_table;
		Entry* m_entry;
		Entry** m_bucket;
		
		void FindNonEmpty()
		{
			for (Entry** end = &m_table->m_ppkHashTable[m_table->m_uiHashSize]; m_bucket != end; ++m_bucket)
				if ((m_entry = *m_bucket)) return;
		};

	public:

		Iterator() : m_table(nullptr), m_entry(nullptr), m_bucket(nullptr) {}
		Iterator(NiTMapBase* table, Entry* entry = nullptr)
			: m_table(table), m_entry(entry), m_bucket(table->m_ppkHashTable) { FindNonEmpty(); }

		~Iterator() { }

		T_Data		Get();
		UInt32		GetKey();
		bool		Next();
		bool		Done();

		//		T_Key Key() const { return m_entry->key; }

		explicit operator bool() const { return m_entry != nullptr; }
		void operator++()
		{
			m_entry = m_entry->m_pkNext;
			if (!m_entry)
			{
				m_bucket++;
				FindNonEmpty();
			}
		}

		T_Data operator->() const { return m_entry->m_val; }
		T_Data& operator*() const { return m_entry->m_val; }
		Iterator& operator=(const Iterator& rhs)
		{
			m_entry = rhs.m_entry;
			return *this;
		}
		bool operator!=(const Iterator& rhs) { return m_entry != rhs.m_entry; }
	};

	Iterator begin() { return Iterator(this); }
	Iterator end() { return Iterator(); }

};

template<class Allocator, class T_Key, class T_Data>
inline NiTMapBase<Allocator, T_Key, T_Data>::~NiTMapBase() {
	RemoveAll();
	NiFree(m_ppkHashTable);
}

template<class Allocator, class T_Key, class T_Data>
inline UInt32 NiTMapBase<Allocator, T_Key, T_Data>::KeyToHashIndex(T_Key key) {
	return (unsigned int)(((size_t)key) % m_uiHashSize);
}

template<class Allocator, class T_Key, class T_Data>
inline bool NiTMapBase<Allocator, T_Key, T_Data>::IsKeysEqual(T_Key key1, T_Key key2) {
	return key1 == key2;
}

template<class Allocator, class T_Key, class T_Data>
inline void NiTMapBase<Allocator, T_Key, T_Data>::SetValue(NiTMapItem<T_Key, T_Data>* pItem, T_Key key, T_Data data) {
	pItem->m_key = key;
	pItem->m_val = data;
}

template<class Allocator, class T_Key, class T_Data>
inline void NiTMapBase<Allocator, T_Key, T_Data>::ClearValue(NiTMapItem<T_Key, T_Data>* pItem) {
}

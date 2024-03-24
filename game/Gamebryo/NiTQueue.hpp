#pragma once

#include "NiMemObject.hpp"

template <class T> class NiTQueue : public NiMemObject {
public:
	NiTQueue(UInt32 uiInitialSize = 1);
	~NiTQueue();

	void Add(T element);
	void AddUnique(T element);
	T Remove();
	T Head() const;
	bool IsEmpty() const;
	UInt32 GetSize() const;
	void Realloc();
	bool IsFull() const;
	void Realloc(UInt32 uiNewSize);

	T* m_pBase;
	UInt32 m_uiAlloced;
	UInt32 m_uiHead;
	UInt32 m_uiTail;
};

template <class T> class NiTObjectQueue : public
NiTQueue<T>
{
public:
	NiTObjectQueue(UInt32 uiInitialSize = 1);
};

template <class T> class NiTPrimitiveQueue : public
NiTQueue<T>
{
public:
	NiTPrimitiveQueue(UInt32 uiInitialSize = 1);
};
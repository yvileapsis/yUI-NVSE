#pragma once

#include "LockFreeQueue.hpp"

template <typename T_Data>
class LockFreePriorityQueue {
public:
	virtual void Destroy(bool abDoFree);
	virtual void IncRefCount(); // return InterlockedIncrement(&this->dwordC);
	virtual void DecRefCount(); // return InterlockedDecrement(&this->dwordC);
	virtual UInt32 GetRefCount(); // return this->dwordC;

	LockFreeQueue<T_Data>** ppQueues;
	UInt32 uiSize;
	UInt32 uiCount;
};

ASSERT_SIZE(LockFreePriorityQueue<UInt32>, 0x10);
#pragma once

#include "LockFreeQueue.hpp"

template <typename T_Data>
class LockFreePriorityQueue {
public:
	virtual				~LockFreePriorityQueue();
	virtual void		IncrementCount();
	virtual void		DecrementCount();
	virtual uint32_t	GetCount() const;

	LockFreeQueue<T_Data>** ppQueues;
	UInt32 uiSize;
	UInt32 uiCount;
};

ASSERT_SIZE(LockFreePriorityQueue<UInt32>, 0x10);
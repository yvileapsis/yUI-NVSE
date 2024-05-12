#pragma once

#include "BSTCommonMessageQueue.hpp"
#include "ScrapHeap.hpp"
#include "BSScrapHeapQueueElem.hpp"
#include "BSSemaphore.hpp"

template <class T_Data>
class BSTCommonScrapHeapMessageQueue : public BSTCommonMessageQueue<T_Data> {
public:
	ScrapHeap*						pScrapHeap;
	BSScrapHeapQueueElem<T_Data>*	pHead;
	BSScrapHeapQueueElem<T_Data>**	ppTail;
	BSSemaphore						semaphore;
};

ASSERT_SIZE(BSTCommonScrapHeapMessageQueue<UInt32>, 0x20);
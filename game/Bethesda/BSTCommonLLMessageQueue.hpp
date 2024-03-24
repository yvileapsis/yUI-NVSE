#pragma once

#include "BSTCommonMessageQueue.hpp"
#include "BSTFreeList.hpp"

template <class T_Data>
class BSTCommonLLMessageQueue : public BSTCommonMessageQueue<T_Data> {
public:
	BSTFreeList<T_Data>*	pMessageList;
	UInt32					unk0C;
	UInt32					unk010;
};

ASSERT_SIZE(BSTCommonLLMessageQueue<void>, 0x14);
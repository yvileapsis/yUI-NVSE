#pragma once

#include "BSTMessageQueue.hpp"

template <class T_Data> 
class BSTCommonMessageQueue : public BSTMessageQueue<T_Data> {
public:
	UInt32 uiLock;
};

ASSERT_SIZE(BSTCommonMessageQueue<UInt32>, 0x8);
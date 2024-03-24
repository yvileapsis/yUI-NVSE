#pragma once

template <class T_Data> class BSTMessageQueue {
public:
	virtual		 ~BSTMessageQueue();
	virtual bool Push(T_Data* apMessage);
	virtual bool TryPush(T_Data* apMessage);
	virtual bool Pop(T_Data* apMessage);
	virtual bool TryPop(T_Data* apMessage);
	virtual bool DoTryPush(T_Data* apMessage);
	virtual bool DoTryPop(T_Data* apMessage);
};

ASSERT_SIZE(BSTMessageQueue<UInt32>, 0x4);
#pragma once

template <typename T_Data>
class BSTFreeList {
public:
	virtual ~BSTFreeList();

	UInt32 uiLock;
	T_Data* pHead;
};

template <typename T_Data, UInt32 Size>
class BSTStaticFreeList : public BSTFreeList<T_Data> {
public:
	T_Data items[Size];
};
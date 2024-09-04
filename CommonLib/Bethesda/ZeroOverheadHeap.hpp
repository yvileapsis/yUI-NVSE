#pragma once

#include "IMemoryHeap.hpp"
#include "BSSpinLock.hpp"

class ZeroOverheadHeap : public IMemoryHeap {
public:
	UInt32		uiSize;
	const char* pName;
	char*		pHeap;
	char*		pCurrentFree;
	UInt32		uiAllocations;
	UInt32		empty[2];
	BSSpinLock	kLock;
};

ASSERT_SIZE(ZeroOverheadHeap, 0x40);
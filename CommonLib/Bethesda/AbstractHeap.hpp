#pragma once

#include "IMemoryHeap.hpp"

struct HeapBlock {
	HeapBlock*	pPrevious = nullptr;
	union {
		HeapBlock*	pNext;
		UInt32		uiSizeAndFlags;
	};
};


class AbstractHeap : public IMemoryHeap {
public:
	virtual void*	DoHeapAllocation(SIZE_T aiSize, SIZE_T aiInitialSize) = 0;
	virtual bool	DoHeapFree(void* apAddress) = 0;
	virtual SIZE_T	CreateMorePages(void* apMem, SIZE_T aiCurrentSize, SIZE_T aiRequestedBytes);
	virtual SIZE_T	CleanExtraPages(void* apMem, SIZE_T aiCurrentSize, SIZE_T aiFreeBytes);
	virtual void	DecommitPages(HeapBlock* apBlock);
	virtual void	CommitPages(int a2, int a3);

	const char*			pName;
	UInt32				uiBlockAlignement;
	UInt32				uiMinFreeBlockSize;
	UInt32				uiPageSize;
	UInt32				uiMemHeapSize;
	UInt32				uiInitialSize;
	UInt32				uiCurrentSize;
	UInt32				uiWastedMemory;
	UInt32				uiMemAllocated;
	UInt32				uiMaxSize;
	UInt32				uiBlockMemAllocated;
	char*				pMemHeap;
	UInt32				uiNumBlocks;
	HeapBlock*			pBlockHead;
	HeapBlock*			pBlockTail;
	UInt32				uiNumFreeBlocks;
	HeapBlock			kLargeFreeList[1024];
	HeapBlock*			pLastBlock;
	HeapBlock			kBlock;
	HeapBlock			unk2050;
	HeapBlock			kSmallFreeList[16];
	CRITICAL_SECTION	kCriticalSection;
};

ASSERT_SIZE(AbstractHeap, 0x20F0);
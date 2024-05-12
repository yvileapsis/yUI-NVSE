#pragma once

#include "IMemoryHeap.hpp"

struct HeapBlock {
	HeapBlock*	pPrevious;
	HeapBlock*	pNextFree;
};


class AbstractHeap : public IMemoryHeap {
public:
	virtual void*	DoHeapAllocation(SIZE_T aiSize, SIZE_T aiInitialSize);
	virtual void	DoHeapFree(void* apAddress);
	virtual void	CreateMorePages(char* apMem, int aiCurrentSize, SIZE_T aiFreeBytes);
	virtual void	CleanExtraPages(char* apMem, int aiCurrentSize, SIZE_T aiFreeBytes);
	virtual void	DecommitPages(HeapBlock* apBlock);
	virtual void	CommitPages(int a2, int a3);

	const char*			pName;
	DWORD				dword8;
	UInt32				uiMinFreeBlockSize;
	UInt32				uiPageSize;
	UInt32				uiMemHeapSize;
	UInt32				uiInitialSize;
	UInt32				uiCurrentSize;
	DWORD				dword20;
	UInt32				uiMemAllocated;
	DWORD				dword28;
	DWORD				dword2C;
	char*				pMemHeap;
	UInt32				uiNumBlocks;
	HeapBlock*			pBlockHead;
	HeapBlock*			pBlockTail;
	UInt32				uiNumFreeBlocks;
	HeapBlock			kBlockList[1024];
	DWORD				unk2044;
	HeapBlock			kBlock;
	HeapBlock			unk2050;
	HeapBlock			kBlockList2058[16];
	CRITICAL_SECTION	kCriticalSection;
};

ASSERT_SIZE(AbstractHeap, 0x20F0);
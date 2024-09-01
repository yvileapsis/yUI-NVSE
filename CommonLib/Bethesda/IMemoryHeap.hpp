#pragma once

struct HeapStats {
	HeapStats() {
		memset(this, 0, sizeof(HeapStats));
	}

	const char* pHeapName;
	DWORD		uiMemHeapSize;
	DWORD		uiMemHeapCommitted;
	DWORD		uiMemAllocatedToBlocks;
	DWORD		uiNumBlocks;
	DWORD		uiNumFreeBlocks;
	DWORD		uiMemFreeInBlocks;
	DWORD		uiMemUsedInBlocks;
	DWORD		uiSmallestFreeBlock;
	DWORD		uiLargestFreeBlock;
	DWORD		uiHeapOverhead;
	DWORD		uiFreeListOverhead;
	DWORD		uiBlockOverhead;
	DWORD		unk34;
	DWORD		uiTotalFree;
};

class IMemoryHeap {
public:
	virtual					~IMemoryHeap();
	virtual const char*		GetName() const;
	virtual void*			Allocate(SIZE_T auiSize, UInt32 auiAlignment);
	virtual void			Deallocate(void* apPointer, SIZE_T);
	virtual bool			PointerInHeap(void* apPointer) const;
	virtual void			CheckHeapIntegrity();
	virtual void			GetHeapStats(HeapStats* apStats, bool abFullBlockInfo) const;
	virtual UInt32			OutputBucketInfo(void*);
	virtual UInt32			OutputBlockInfo(void* apFile, bool abOutputWalk);
	virtual SIZE_T			Size(void* apPointer, int = 0) const;
	virtual SIZE_T			TotalSize(void* apPointer) const;
	virtual void			Unk_11();
};
#pragma once

struct HeapStats {
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
	DWORD		unk28;
	DWORD		unk2C;
	DWORD		uiBlockOverhead;
	DWORD		unk34;
	DWORD		uiTotalFree;
};

class IMemoryHeap {
public:
	virtual					~IMemoryHeap();
	virtual const char*		GetName();
	virtual void*			BaseAllocate(SIZE_T auiSize, UInt32 auiAlignment);
	virtual void			BaseFree(void* apPointer, SIZE_T);
	virtual bool			PointerInHeap(void* apPointer) const;
	virtual void			CheckHeapIntegrity();
	virtual void			GetHeapStats(HeapStats* apStats, bool abFullBlockInfo);
	virtual UInt32			OutputBucketInfo(void*);
	virtual UInt32			OutputBlockInfo(void* apFile, bool abOutputWalk);
	virtual int				GetBlockAllocContext(void* apMemory, int = 0);
	virtual int				GetBlockAllocCheckPoint(int);
	virtual void			Unk_11();
};
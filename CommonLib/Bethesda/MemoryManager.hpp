#pragma once

#include "BSMap.hpp"
#include "MemoryContexts.hpp"

class BSExtraData;
class ExtraDataList;
class NiNode;
class ScriptLocals;
class TESObjectREFR;
class MemoryPool;
class IMemoryHeap;
struct HeapStats;
class ScrapHeap;
class IMemoryManagerFile;

struct MemoryManager {
    bool						bInitialized;
    UInt16						usNumHeaps;
    UInt16						usNumPhysicalHeaps;
    IMemoryHeap**				ppHeaps;
    IMemoryHeap*				pHeapsByContext[66];
	BSMap<UInt32, ScrapHeap*>	kThreadHeapMap;
	IMemoryHeap**				ppPhysicalHeaps;
    bool						bSpecialHeaps;
    bool						bAllowPoolUse;
    UInt32						uiSysAllocBytes;
	UInt32						uiMallocBytes;
	UInt32						uiAlignmentForPools;

	static MemoryManager* GetSingleton();

	static MemoryPool* GetPool(UInt32 auiIndex);
    
	ScrapHeap* GetThreadScrapHeap();

	[[nodiscard]]
	__declspec(restrict) __declspec(allocator) void* Allocate(size_t aSize);
	[[nodiscard]]
	__declspec(restrict) __declspec(allocator) void* Reallocate(void* apOldMem, size_t auiNewSize);

    void Deallocate(void* apMemory);

    static void* AllocateScrap(size_t aSize, UInt32 aAlignment);
    static void DeallocateScrap(void* apMemory);

	static void EnterContext(UInt32& apPrevious, UInt32 auiNew);
	static void LeaveContext(UInt32& apPrevious);

	void SpecifyMemoryLayout();
	void SpecifyPools();

	void CreatePool(UInt32 auiPoolNumber, UInt32 auiSize, const char* apName);
	static MemoryPool* SelectPoolFromID(UInt32 auiID);

	size_t Size(void* apMemory) const;

	[[nodiscard]]
	__declspec(restrict) __declspec(allocator) void* DefaultAllocate(UInt32 auiSize, bool abTrack = false);
	void Free(void* apMemory, bool abTrack = false);
	IMemoryHeap* GetHeapByIndex(UInt32 auiIndex) const;
	IMemoryHeap* GetHeapForPointer(void* apMem) const;
	IMemoryHeap* GetHeapForPhysicalPointer(void* apMem) const;
	UInt32 ProcessMemoryProblemImpl(IMemoryHeap* apMemoryHeap, UInt32 aiMemoryPass, bool* apbAllowSystemAllocs);

	static void FreeUnusedPagesForAllPools();

	bool GetHeapStats(UInt32 auiIndex, bool abFullBlockInfo, HeapStats* apStats) const;
	bool GetPhysicalHeapStats(UInt32 auiPhysicalHeapIndex, bool abFullBlockInfo, HeapStats* apStats) const;
};
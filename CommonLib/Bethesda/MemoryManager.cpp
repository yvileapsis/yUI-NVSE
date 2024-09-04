#include "MemoryManager.hpp"
#include "IMemoryHeap.hpp"
#include "ScrapHeap.hpp"
#include "TLSData.hpp"

MemoryPool* MemoryManager::GetPool(UInt32 auiIndex) {
	return ((MemoryPool**)0x11F63B8)[auiIndex];
}

MemoryManager* MemoryManager::GetSingleton() {
	return &*(MemoryManager*)0x11F6238;
}

// 0xAA42E0
ScrapHeap* MemoryManager::GetThreadScrapHeap() {
	return ThisStdCall<ScrapHeap*>(0xAA42E0, MemoryManager::GetSingleton());
}

// 0xAA3E40
void* MemoryManager::Allocate(size_t aSize) {
	return ThisStdCall<void*>(0xAA3E40, MemoryManager::GetSingleton(), aSize);
}

// 0xAA4150
void* MemoryManager::Reallocate(void* apOldMem, size_t auiNewSize) {
	return ThisStdCall<void*>(0xAA4150, MemoryManager::GetSingleton(), apOldMem, auiNewSize);
}

// 0xAA4060
void MemoryManager::Deallocate(void* apMemory) {
	ThisStdCall(0xAA4060, MemoryManager::GetSingleton(), apMemory);
}

void MemoryManager::EnterContext(UInt32& apPrevious, UInt32 auiNew) {
	TLSData* pTLSData = TLSData::GetTLSData();
	apPrevious = pTLSData->uiHeapIndex;
	pTLSData->uiHeapIndex = auiNew;
}

void MemoryManager::LeaveContext(UInt32& apPrevious) {
	TLSData::SetHeapIndex(apPrevious);
}

// 0x8286A6B8 - Xbox
bool MemoryManager::GetHeapStats(UInt32 auiIndex, bool abFullBlockInfo, HeapStats* apStats) const {
	UInt32 iNumHeaps = usNumHeaps;
	IMemoryHeap* pHeap = nullptr;

	if (auiIndex >= iNumHeaps) {
		auiIndex = auiIndex - iNumHeaps;
		if (auiIndex >= usNumPhysicalHeaps)
			return false;

		pHeap = ppPhysicalHeaps[auiIndex];
	}
	else {
		pHeap = ppHeaps[auiIndex];
	}

	if (!pHeap)
		return false;

	pHeap->GetHeapStats(apStats, abFullBlockInfo);
	return true;
}
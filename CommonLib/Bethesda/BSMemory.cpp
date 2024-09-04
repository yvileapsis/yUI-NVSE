#include "BSMemory.hpp"
#include "MemoryManager.hpp"

// 0x401000
void* BSNew(size_t stSize) {
    return MemoryManager::GetSingleton()->Allocate(stSize);
}

// 0x553320
void* BSNewAligned(size_t stAlign, size_t stSize) {
    char* pMemory = static_cast<char*>(MemoryManager::GetSingleton()->Allocate(stSize));
    UInt32 uiAlignment = stAlign - (reinterpret_cast<UInt8>(pMemory) & (stAlign - 1));
    pMemory[UInt8(uiAlignment) - 1] = uiAlignment;
    return &pMemory[UInt8(uiAlignment)];
}

// 0x42F5D0
void* BSReallocate(void* pvMem, size_t stSize) {
    return MemoryManager::GetSingleton()->Reallocate(pvMem, stSize);
}

// 0x401030
void BSFree(void* pvMem) {
    MemoryManager::GetSingleton()->Deallocate(pvMem);
}

SIZE_T BSSize(void* pvMem) {
	return MemoryManager::GetSingleton()->Size(pvMem);
}
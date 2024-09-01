#include "NiMemory.hpp"
#include "NiMemManager.hpp"

// 0xAA13E0
void* NiNew(size_t stSize) {
    if (!stSize)
        stSize = 1;

    size_t stAlignment = 4;
    return NiMemManager::GetSingleton()->m_pkAllocator->Allocate(stSize, stAlignment, NI_OPER_NEW, true, 0, -1, 0);
}

// 0xAA1070
void* NiAlloc(size_t stSize) {
    if (!stSize)
        stSize = 1;

    size_t stAlignment = 4;
    return NiMemManager::GetSingleton()->m_pkAllocator->Allocate(stSize, stAlignment, NI_MALLOC, false, 0, -1, 0);
}

// 0xAA10B0
void* NiAlignedAlloc(size_t stSize, size_t stAlignment) {
    if (!stSize)
        stSize = 1;

    return NiMemManager::GetSingleton()->m_pkAllocator->Allocate(stSize, stAlignment, NI_ALIGNEDMALLOC, false, 0, -1, 0);
}

// 0xAA10F0
void NiFree(void* pvMem) {
    if (pvMem)
        NiMemManager::GetSingleton()->m_pkAllocator->Deallocate(pvMem, NI_FREE, -1);
}

// 0xAA1110
void NiAlignedFree(void* pvMem) {
    if (pvMem)
        NiMemManager::GetSingleton()->m_pkAllocator->Deallocate(pvMem, NI_ALIGNEDFREE, -1);
}

// 0xAA1460
void NiDelete(void* pvMem, size_t stElementSize) {
    if (pvMem)
        NiMemManager::GetSingleton()->m_pkAllocator->Deallocate(pvMem, NI_OPER_DELETE, stElementSize);
}

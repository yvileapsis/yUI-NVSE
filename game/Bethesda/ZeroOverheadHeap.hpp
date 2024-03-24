#pragma once

#include "IMemoryHeap.hpp"
#include "BSSpinLock.hpp"

class ZeroOverheadHeap : public IMemoryHeap {
public:
	DWORD		uiSize;
	const char* pName;
	DWORD		pHeap;
	DWORD		pCurrentFree;
	DWORD		iNumBlocks;
	BYTE		gap18[8];
	BSSpinLock	kLock;

	static ZeroOverheadHeap* Create(size_t auiSize, const char* apName);
};

ASSERT_SIZE(ZeroOverheadHeap, 0x40);
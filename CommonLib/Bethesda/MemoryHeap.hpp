#pragma once

#include "AbstractHeap.hpp"

class MemoryHeap : public AbstractHeap {
public:
	bool bSupportsSwapping;
	bool bDeletingHeap;

	static MemoryHeap* Create(size_t auiSize, size_t auiInitialSize, const char* apName, bool abSupportsSwapping);
};

ASSERT_SIZE(MemoryHeap, 0x20F4);
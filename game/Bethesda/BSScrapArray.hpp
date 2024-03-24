#pragma once

#include "BSSimpleArray.hpp"
#include "ScrapHeap.hpp"

template <typename T>
class BSScrapArray : public BSSimpleArray<T> {
public:
	ScrapHeap* pHeap;
};

ASSERT_SIZE(BSScrapArray<UInt32>, 0x14);
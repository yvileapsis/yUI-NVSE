#pragma once

#include "BSHash.hpp"

class BSFileEntry : public BSHash {
public:
	SInt32 iSize;
	SInt32 iOffset;
};

ASSERT_SIZE(BSFileEntry, 0x10);
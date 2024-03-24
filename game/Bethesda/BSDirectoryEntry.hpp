#pragma once

#include "BSHash.hpp"
#include "BSFileEntry.hpp"

class BSDirectoryEntry : public BSHash {
public:
	UInt32			uiFiles;
	BSFileEntry*	pFiles;
};

ASSERT_SIZE(BSDirectoryEntry, 0x10);
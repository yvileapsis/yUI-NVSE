#pragma once

#include "BSSpinLock.hpp"
#include "BSMemObject.hpp"

class MemoryPool : public BSMemObject {
public:
	MemoryPool(UInt32 auiPoolNumber, UInt32 auiSize, const char* apName);
	~MemoryPool();

	struct Block {
		Block* pData = 0;
		Block* pNext = 0;
	};


	const char* pName;
	void*		pAllocBase;
	Block*		pLastBlock;
	BYTE		gapC[20];
	BSSpinLock	kLock;
	UInt32		uiBlockSize;
	UInt32		uiTotalAllocations;
	SInt16*		pPages;
	UInt32		uiPageCount;
	UInt32		uiSize;
	UInt32		uiFreeBlocks;
	UInt32		uiActiveAllocations;
	UInt32		dword5C;
	UInt32		dword60;
	UInt32		unk64;
	UInt32		unk68;
	UInt32		unk6C;
	UInt32		unk70;
	UInt32		unk74;
	UInt32		unk78;
	UInt32		unk7C;

	static constexpr UInt32 PAGE_SIZE = 4096;

	UInt32	GetBlocksPerPage() const {
		return PAGE_SIZE / uiBlockSize;
	};
};

ASSERT_SIZE(MemoryPool, 0x80);
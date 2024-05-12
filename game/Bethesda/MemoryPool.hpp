#pragma once

#include "BSSpinLock.hpp"
#include "BSMemObject.hpp"

class MemoryPool : public BSMemObject {
public:
	MemoryPool(UInt32 auiPoolNumber, UInt32 auiSize, const char* apName);
	~MemoryPool();

	struct BlockNode {
		BYTE*	pData;
		void*	pNext;
	};


	const char* pName;
	void*		pVirtMemory;
	BlockNode*	pMemoryBlocks;
	BYTE		gapC[20];
	BSSpinLock	kLock;
	UInt32		uiNumber;
	UInt32		uiUnk44;
	SInt16*		pData;
	UInt32		uiDataCount;
	UInt32		uiSize;
	UInt32		uiBlockCount;
	UInt32		uiUnk58;
	UInt32		dword5C;
	UInt32		dword60;
	UInt32		unk64;
	UInt32		unk68;
	UInt32		unk6C;
	UInt32		unk70;
	UInt32		unk74;
	UInt32		unk78;
	UInt32		unk7C;

	void*	Allocate(size_t aSize);
	void	Free(void* apMemory);

private:
	void	Register();
	void	Unregister();

	void	AllocateVirtualMemory();
	BOOL	ReleaseVirtualMemory();
};

ASSERT_SIZE(MemoryPool, 0x80);
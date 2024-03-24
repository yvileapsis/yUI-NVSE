#pragma once

#include "IMemoryManagerFile.hpp"
#include "BSSystemFile.hpp"

class MemoryManagerFile : public IMemoryManagerFile {
public:
	BSSystemFile kFile;

	UInt32	Read(void* apBuffer, UInt32 auiSize) override;
	UInt32	Write(const void* apBuffer, UInt32 auiSize) override;
	UInt32	Size() override;
	void	Seek(UInt32 auiOffset) override;
};

ASSERT_SIZE(MemoryManagerFile, 0x18);
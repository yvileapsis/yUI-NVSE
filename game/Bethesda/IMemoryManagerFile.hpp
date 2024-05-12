#pragma once

class IMemoryManagerFile {
public:
	IMemoryManagerFile() {};
	virtual ~IMemoryManagerFile() {};
	virtual UInt32	Read(void* apBuffer, UInt32 auiSize) = 0;
	virtual UInt32	Write(const void* apBuffer, UInt32 auiSize) = 0;
	virtual UInt32	Size() = 0;
	virtual void	Seek(UInt32 auiOffset) = 0;
};
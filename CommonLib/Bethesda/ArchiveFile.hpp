#pragma once

#include "BSFile.hpp"
#include "NiSmartPointer.hpp"

class Archive;

class ArchiveFile : public BSFile {
public:
	ArchiveFile(const char* apName, Archive* apArchive, UInt32 auiOffset, UInt32 auiSize, UInt32 auiBufferSize);
	~ArchiveFile() override;

	NiPointer<Archive>	spArchive;
	UInt32				aiOffset;


	void	SetEndianSwap(bool abDoSwap) override;
	void	Seek(SInt32 aiOffset, SInt32 aiWhence) override;
	bool	Open(int = 0, bool abTextMode = false) override;
	UInt32	GetSize() override;
	UInt32	WriteString(BSStringT<char>& arString, bool abBinary) override;
	UInt32	WriteStringAlt(BSStringT<char>& arString, bool abBinary) override;
	bool	IsReadable() override;
	UInt32	DoRead(void* apBuffer, UInt32 auiBytes) override;
	UInt32	DoWrite(const void* apBuffer, UInt32 auiBytes) override;

	UInt32 ReadFromBuffer(char* apBuffer, UInt32 auiBytes);
	UInt32 ReadFromArchive(char* apBuffer, UInt32 auiBytes, UInt32 auiOffset);

	static UInt32 __cdecl ReadNoSwap(NiBinaryStream* apThis, void* apvBuffer, UInt32 auiBytes, UInt32* apuiComponentSizes, UInt32 auiNumComponents);
};

ASSERT_SIZE(ArchiveFile, 0x160);
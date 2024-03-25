#pragma once

#include "NiFile.hpp"
#include "BSString.hpp"

class BSFile : public NiFile {
public:
	BSFile();
	BSFile(const char* apName, OpenMode aeMode, UInt32 auiBufferSize, bool abTextMode);
	~BSFile() override;

	virtual bool	Open(int = 0, bool abTextMode = false);
	virtual bool	OpenByFilePointer(FILE* apFile);
	virtual UInt32	GetSize();
	virtual UInt32	ReadString(BSStringT<char>& arString, UInt32 auiMaxLength);
	virtual UInt32	ReadStringAlt(BSStringT<char>& arString, UInt32 auiMaxLength);
	virtual UInt32	GetLine(char* apBuffer, UInt32 auiMaxBytes, UInt8 aucMark);
	virtual UInt32	WriteString(BSStringT<char>& arString, bool abBinary);
	virtual UInt32	WriteStringAlt(BSStringT<char>& arString, bool abBinary);
	virtual bool	IsReadable();
	virtual UInt32	DoRead(void* apBuffer, UInt32 auiBytes);
	virtual UInt32	DoWrite(const void* apBuffer, UInt32 auiBytes);

	bool		bUseAuxBuffer;
	void*		pAuxBuffer;
	SInt32		iAuxTrueFilePos;
	DWORD		dword3C;
	DWORD		dword40;
	char		cFileName[260];
	UInt32		uiResult;
	UInt32		uiIOSize;
	UInt32		uiTrueFilePos;
	UInt32		uiFileSize;

	void		SetEndianSwap(bool abDoSwap) override;
	void		Seek(SInt32 aiOffset, SInt32 aiWhence);
	const char*	GetFilename() const override;
	UInt32		GetFileSize() override;

	void Close();

	void CheckIsGood();

	bool ChangeBufferSize(UInt32 auiSize);

	UInt32 DiskRead(void* apBuffer, UInt32 auiBytes);

	UInt32 ReadBuffer(void* apData, UInt32 auiSize);

	static UInt32 __cdecl ReadAndSwap(NiBinaryStream* apThis, void* apvBuffer, UInt32 auiBytes, UInt32* apuiComponentSizes, UInt32 auiNumComponents);
	static UInt32 __cdecl WriteAndSwap(NiBinaryStream* apThis, const void* apvBuffer, UInt32 auiBytes, UInt32* apuiComponentSizes, UInt32 auiNumComponents);

	static UInt32 __cdecl ReadNoSwap(NiBinaryStream* apThis, void* apvBuffer, UInt32 auiBytes, UInt32* apuiComponentSizes, UInt32 auiNumComponents);
	static UInt32 __cdecl WriteNoSwap(NiBinaryStream* apThis, const void* apvBuffer, UInt32 auiBytes, UInt32* apuiComponentSizes, UInt32 auiNumComponents);
};

ASSERT_SIZE(BSFile, 0x158);
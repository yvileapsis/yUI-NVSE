#pragma once

#include "NiBinaryStream.hpp"

class NiFile : public NiBinaryStream {
public:
	enum OpenMode {
		READ_ONLY = 0x0,
		WRITE_ONLY = 0x1,
		APPEND_ONLY = 0x2,
	};

	NiFile();
	NiFile(const char* apFileName, OpenMode aeMode, UInt32 auiBufferAllocSize);
	~NiFile() override;

	virtual void		Seek(SInt32 aiOffset, SInt32 aiWhence);
	virtual const char*	GetFilename() const;
	virtual UInt32		GetFileSize();

	UInt32		m_uiBufferAllocSize;
	UInt32		m_uiBufferReadSize;
	UInt32		m_uiPos;
	UInt32		m_uiAbsolutePos;
	char*		m_pBuffer;
	FILE*		m_pFile;
	OpenMode	m_eMode;
	bool		m_bGood;

	static SInt32 ms_iSeekCur;

	operator bool() override;
	void	Seek(SInt32 aiNumBytes) override;
	void	SetEndianSwap(bool abDoSwap) override;

	bool Flush();

	char* GetBuffer() const;

	static UInt32 __cdecl ReadAndSwap(NiBinaryStream* apThis, void* apvBuffer, UInt32 auiBytes, UInt32* apuiComponentSizes, UInt32 auiNumComponents);
	static UInt32 __cdecl WriteAndSwap(NiBinaryStream* apThis, const void* apvBuffer, UInt32 auiBytes, UInt32* apuiComponentSizes, UInt32 auiNumComponents);
	
	static UInt32 __cdecl ReadNoSwap(NiBinaryStream* apThis, void* apvBuffer, UInt32 auiBytes, UInt32* apuiComponentSizes, UInt32 auiNumComponents);
	static UInt32 __cdecl WriteNoSwap(NiBinaryStream* apThis, const void* apvBuffer, UInt32 auiBytes, UInt32* apuiComponentSizes, UInt32 auiNumComponents);


	UInt32 FileRead(void* apBuffer, UInt32 auiBytes);
	UInt32 FileWrite(const void* apBuffer, UInt32 auiBytes);

	UInt32 DiskRead(void* apBuffer, UInt32 auiBytes);
	UInt32 DiskWrite(const void* apBuffer, UInt32 auiBytes);

	UInt32 ReadBuffer(void* pvBuffer, UInt32 uiBytes, UInt32* puiComponentSizes, UInt32 uiNumComponents);
};

ASSERT_SIZE(NiFile, 0x30);
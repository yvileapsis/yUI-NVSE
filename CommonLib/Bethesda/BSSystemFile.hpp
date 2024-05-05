#pragma once

class BSSystemFile {
public:
	enum AccessMode {
		AM_RDONLY   = 0,
		AM_RDWR	 = 1,
		AM_WRONLY   = 2
	};

	enum OpenMode {
		OM_NONE	 = 0,
		OM_APPEND   = 1,
		OM_CREAT	= 2,
		OM_TRUNC	= 3,
		OM_EXCL	 = 4
	};

	enum SeekMode {
		SM_SET = 0,
		SM_CUR = 1,
		SM_END = 2
	};

	enum ErrorCode {
		EC_NONE			 = 0,
		EC_INVALID_FILE	 = 1,
		EC_NOT_EXIST		= 2,
		EC_ALREADY_EXISTS   = 3,
		EC_FAILED		   = 4
	};

	enum FileOperation {
		FO_OPEN	 = 0,
		FO_READ	 = 1,
		FO_WRITE	= 2,
		FO_SEEK	 = 3
	};

	BSSystemFile(const char* apName, AccessMode aeWriteType, OpenMode aeOpenMode, bool abSynchronousNoBuffer);
	~BSSystemFile();

	DWORD	eState;
	DWORD	uiWriteType;
	DWORD	eOpenMode;
	bool	bSynchronousNoBuffer;
	HANDLE	hFile;

	UInt32 DoOpen(const char* apName);
	UInt32 DoRead(void* apBuffer, UInt32 auiBytes, UInt32* auiRead) const;
	UInt32 DoWrite(const void* apBuffer, UInt32 auiBytes, UInt32* auiWrite) const;
	UInt32 DoSeek(LARGE_INTEGER aiOffset, UInt32 aiWhence, LARGE_INTEGER& arPos) const;
	UInt32 DoGetSize(LARGE_INTEGER& arSize) const;
	bool   DoClose() const;
	UInt32 DoFlush();

	static UInt32 GetSizeOfFile(const char* apName, LARGE_INTEGER& arSize);

	UInt32 Read(void* apBuffer, UInt32 auiBytes, UInt32* auiRead);
	UInt32 Write(const void* apBuffer, UInt32 auiBytes, UInt32* auiWrite);
	UInt32 Seek(LARGE_INTEGER aiOffset, UInt32 aiWhence, LARGE_INTEGER& arPos);
	UInt32 GetSize(LARGE_INTEGER& arSize);
};

ASSERT_SIZE(BSSystemFile, 0x14);
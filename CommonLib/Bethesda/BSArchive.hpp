#pragma once

#include "BSDirectoryEntry.hpp"

class BSArchiveHeader {
public:
	BSArchiveHeader() {
		uiTag = 0x415342;
		uiVersion = 104;
		uiHeaderSize = sizeof(BSArchiveHeader);
		uiFlags.field = 0;
		uiDirectories = 0;
		uiFiles = 0;
		uiDirectoryNamesLength = 0;
		uiFileNamesLength = 0;
		usArchiveType = 0;
	};
	~BSArchiveHeader() {};

	UInt32		uiTag;
	UInt32		uiVersion;
	UInt32		uiHeaderSize;
	Bitfield32	uiFlags;
	UInt32		uiDirectories;
	UInt32		uiFiles;
	UInt32		uiDirectoryNamesLength;
	UInt32		uiFileNamesLength;
	UInt16		usArchiveType;

	enum Flags {
		DIRSTRINGS						= 1 << 0,
		FILESTRINGS						= 1 << 1,
		COMPRESSED						= 1 << 2,
		RETAIN_DIRECTORY_NAMES			= 1 << 3,
		RETAIN_FILE_NAMES				= 1 << 4,
		RETAIN_FILE_NAME_OFFSETS		= 1 << 5,
		XBOX_ARCHIVE					= 1 << 6,
		RETAIN_STRINGS_DURING_STARTUP	= 1 << 7,
		EMBEDDED_FILE_NAMES				= 1 << 8,
		XBOX_COMPRESSED					= 1 << 9,
		NEXT_FLAG						= 1 << 10,
	};

	void SetCompressed(bool bCompressed) { uiFlags.SetBit(COMPRESSED, bCompressed); }
	bool IsCompressed() const { return uiFlags.IsSet(COMPRESSED); }

	void SetXBoxArchive(bool bXBoxArchive) { uiFlags.SetBit(XBOX_ARCHIVE, bXBoxArchive); }
	bool IsXBoxArchive() const { return uiFlags.IsSet(XBOX_ARCHIVE); }

	void SetXBoxCompressed(bool bXBoxCompressed) { uiFlags.SetBit(XBOX_COMPRESSED, bXBoxCompressed); }
	bool IsXBoxCompressed() const { return uiFlags.IsSet(XBOX_COMPRESSED); }

	void SetEmbeddedFileNames(bool bEmbeddedFileNames) { uiFlags.SetBit(EMBEDDED_FILE_NAMES, bEmbeddedFileNames); }
	bool HasEmbeddedFileNames() const { return uiFlags.IsSet(EMBEDDED_FILE_NAMES); }

	void SetDirectoryStrings(bool bDirectoryStrings) { uiFlags.SetBit(DIRSTRINGS, bDirectoryStrings); }
	bool HasDirectoryStrings() const { return uiFlags.IsSet(DIRSTRINGS); }

	void SetFileStrings(bool bFileStrings) { uiFlags.SetBit(FILESTRINGS, bFileStrings); }
	bool HasFileStrings() const { return uiFlags.IsSet(FILESTRINGS); }

	void SetRetainDirectoryNames(bool bRetainDirectoryNames) { uiFlags.SetBit(RETAIN_DIRECTORY_NAMES, bRetainDirectoryNames); }
	bool RetainDirectoryNames() const { return uiFlags.IsSet(RETAIN_DIRECTORY_NAMES); }

	void SetRetainFileNames(bool bRetainFileNames) { uiFlags.SetBit(RETAIN_FILE_NAMES, bRetainFileNames); }
	bool RetainFileNames() const { return uiFlags.IsSet(RETAIN_FILE_NAMES); }

	void SetRetainFileOffsets(bool bRetainFileOffsets) { uiFlags.SetBit(RETAIN_FILE_NAME_OFFSETS, bRetainFileOffsets); }
	bool RetainFileOffsets() const { return uiFlags.IsSet(RETAIN_FILE_NAME_OFFSETS); }

	void SetRetainStringsDuringStartup(bool bRetainStringsDuringStartup) { uiFlags.SetBit(RETAIN_STRINGS_DURING_STARTUP, bRetainStringsDuringStartup); }
	bool RetainStringsDuringStartup() const { return uiFlags.IsSet(RETAIN_STRINGS_DURING_STARTUP); }

	void SetNextFlag(bool bNextFlag) { uiFlags.SetBit(NEXT_FLAG, bNextFlag); }
	bool HasNextFlag() const { return uiFlags.IsSet(NEXT_FLAG); }
};

ASSERT_SIZE(BSArchiveHeader, 0x24);

class BSArchive : public BSArchiveHeader {
public:
	BSArchive() { pDirectories = 0; };
	~BSArchive() {};

	BSDirectoryEntry* pDirectories;
};

ASSERT_SIZE(BSArchive, 0x28);
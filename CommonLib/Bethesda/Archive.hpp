#pragma once
#include "BSArchive.hpp"
#include "ArchiveFile.hpp"
#include "NiRefObject.hpp"
#include "BSFileEntry.hpp"
#include "BSSimpleList.hpp"
#include "NiCriticalSection.hpp"

enum ARCHIVE_TYPE {
	ARCHIVE_TYPE_ALL_		= 0xFFFFFFFF,
	ARCHIVE_TYPE_ALL		= 0xFFFF,
	ARCHIVE_TYPE_MESHES		= 0x1,
	ARCHIVE_TYPE_TEXTURES	= 0x2,
	ARCHIVE_TYPE_MENUS		= 0x4,
	ARCHIVE_TYPE_SOUNDS		= 0x8,
	ARCHIVE_TYPE_VOICES		= 0x10,
	ARCHIVE_TYPE_SHADERS	= 0x20,
	ARCHIVE_TYPE_TREES		= 0x40,
	ARCHIVE_TYPE_FONTS		= 0x80,
	ARCHIVE_TYPE_MISC		= 0x100,
	ARCHIVE_TYPE_COUNT		= 9,
};


enum ARCHIVE_TYPE_INDEX {
	ARCHIVE_TYPE_INDEX_MESHES	= 0,
	ARCHIVE_TYPE_INDEX_TEXTURES = 1,
	ARCHIVE_TYPE_INDEX_MENUS	= 2,
	ARCHIVE_TYPE_INDEX_SOUNDS	= 3,
	ARCHIVE_TYPE_INDEX_VOICES	= 4,
	ARCHIVE_TYPE_INDEX_SHADERS	= 5,
	ARCHIVE_TYPE_INDEX_TREES	= 6,
	ARCHIVE_TYPE_INDEX_FONTS	= 7,
	ARCHIVE_TYPE_INDEX_MISC		= 8,
	ARCHIVE_TYPE_INDEX_COUNT	= 9,
};

struct ArchiveTypeExtension {
	char			cExtension[4];
	ARCHIVE_TYPE	eArchiveType;
};

NiSmartPointer(Archive);

// 0x1D0
class Archive : public BSFile
{
public:
	NiRefObject			kRefObject;
	BSArchive			kArchive;
	FILETIME			kArchiveFileTime;
	UInt32				uiFileNameArrayOffset;
	UInt32				uiLastDirectoryIndex;
	UInt32				uiLastFileIndex;
	CRITICAL_SECTION	kArchiveCriticalSection;
	UInt8				cArchiveFlags;
	const char*			pDirectoryStringArray;
	UInt32*				pDirectoryStringOffsets;
	const char*			pFileNameStringArray;
	UInt32**			pFileNameStringOffsets;
	UInt32				uiID;
	UInt32				unk6C;

	enum Flags {
		DISABLED = 1 << 0,
		PRIMARY = 1 << 2,
		SECONDARY = 1 << 3,
		HAS_DIRECTORY_STRINGS = 1 << 4,
		HAS_FILE_STRINGS = 1 << 5,
	};

	static UInt32* const uiCount;

	void SetDisabled(bool abDisabled);
	bool IsDisabled() const;

	void SetPrimary(bool abPrimary);
	bool IsPrimary() const;

	void SetSecondary(bool abSecondary);
	bool IsSecondary() const;

	void SetHasDirectoryStrings(bool abHasDirectoryStrings);
	bool HasDirectoryStrings() const;

	void SetHasFileStrings(bool abHasFileStrings);
	bool HasFileStrings() const;

	void DeleteDirectoryStringArray();
	void DeleteFilenameStringArray(bool abKeepOffsets);
	void DeleteFileData();

	bool CheckValidFile(BSFileEntry* apFileEntry, bool abSkipSecondaryArchiveCheck) const;

	bool InvalidateFile(BSHash& arDirectoryHash, BSHash& arFileNameHash, const char* apPath = nullptr);
	bool InvalidateFile(UInt32 auiDirectoryIndex, BSHash& arFileNameHash, const char* apPath = nullptr);

	bool GetLoadStrings();
	bool FindNewerAndInvalidate();
	bool FindFile(BSHash& arDirectoryHash, BSHash& arFileNameHash, UInt32& arDirectoryID, UInt32& arFileID, const char* apFileName);
	bool FindDirectory(BSHash& arDirectoryHash, UInt32& aruiDirectoryIndex, const char* apFileName);
	bool FindFileInDirectory(UInt32 auiDirectoryIndex, BSHash& arFileNameHash, UInt32& aruiFileIndex, const char* apFileName, bool abSkipSecondaryArchiveCheck);
	BSFileEntry* GetFileEntryForFile(BSHash& arDirectoryHash, BSHash& arFileNameHash, const char* apFileName);

	ArchiveFile* GetFile(UInt32 auiDirectoryIndex, UInt32 auiFileIndex, UInt32 auiBufferSize, const char* apFileName);

	SInt32 GetDirectoryIndexForFileEntry(BSFileEntry* apFileEntry);
	UInt32 GetFileIndexForFileEntryFromDirectory(UInt32 auiDirectory, BSFileEntry* apFileEntry) const;
	const char* GetFileString(UInt32 auiDirectoryIndex, UInt32 auiFileIndex);
	const char* GetDirectoryString(UInt32 auiDirectoryIndex);
	const char* GetFileNameForFileEntry(BSFileEntry* apFileEntry);
};
static_assert(sizeof(Archive) == 0x1D0);

typedef BSSimpleList<ArchivePtr> ArchiveList;

class ArchiveManager {
public:
	static NiCriticalSection* const pCriticalSection;
	static ArchiveList** pArchiveList;
	static ArchiveTypeExtension* const pArchiveTypeExtensionsMap;

	static UInt32* const uiArchiveFileBufferSizeS;
	static bool* const bInvalidateOlderFiles;

	static void __cdecl AddToFileList(BSSimpleList<const char*>* apFileNameList, const char* apSearchName, const char* apBaseFilename, ARCHIVE_TYPE aeArchiveType);
	static BSHash** __cdecl AddArchiveToFileList(Archive* apArchive, UInt32 aiNumFiles, BSHash** apHashArray, BSHash& arDirectoryHash, BSHash& arSearchHash, BSSimpleList<const char*>* apFileNameList, const char* apBaseFilename);

	static void __cdecl LoadInvalidationFile(const char* apFileName);
	static bool __cdecl InvalidateFile(const char* apFileName);
	static void __cdecl InvalidateFileInArchives(BSHash& arDirectoryHash, BSHash& arFileHash, UInt16 aiArchiveTypes);

	static Archive* __cdecl GetMasterArchive(ARCHIVE_TYPE_INDEX aeTypeIndex);
	static Archive* __cdecl GetArchiveForFile(const char* apFileName, ARCHIVE_TYPE aiArchiveType);
	static Archive* __cdecl GetArchiveForFileEntry(BSFileEntry* apFileEntry, ARCHIVE_TYPE aiArchiveType);

	static bool OpenMasterArchives();
	static Archive* __cdecl OpenArchive(const char* apArchiveName, UInt16 aiForceArchiveType, bool abInvalidateOtherArchives);

	static bool __cdecl FindFile(const char* apFileName, ARCHIVE_TYPE aiArchiveType);

	static ArchiveFile* __cdecl GetFile(const char* apFileName, UInt32 aiBufferSize, ARCHIVE_TYPE aeArchiveType);
	static ArchiveFile* __cdecl GetArchiveFile(const char* apFileName, UInt32 aiBufferSize, ARCHIVE_TYPE aeArchiveType);
	static BSFileEntry* __cdecl GetFileEntryForFile(ARCHIVE_TYPE_INDEX aeArchiveTypeIndex, BSHash& arDirectoryHash, BSHash& arFileNameHash, const char* apFileName);

	static bool __cdecl WildCardMatch(char* apSearchName, BSHash* apHash);


	static ARCHIVE_TYPE __cdecl GetArchiveTypeFromFileName(const char* apFileName);
	static ARCHIVE_TYPE __cdecl GetArchiveTypeFromFileExtension(const char* apExtension);

	static bool __cdecl GetFileNameForSmallestFileInDirectory(const char* apDirectory, char* apFileName, ARCHIVE_TYPE aeArchiveType);
	static bool __cdecl GetRandomFileNameForDirectory(const char* apDirectory, char* apFileName, ARCHIVE_TYPE aeArchiveType);


	static const char* __cdecl TrimFileName(const char* apFileName);


	static inline void Lock() {
		pCriticalSection->Lock();
	}

	static inline void Unlock() {
		pCriticalSection->Unlock();
	}
};
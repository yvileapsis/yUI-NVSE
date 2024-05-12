#pragma once

#include "Archive.hpp"
#include "BSSimpleArray.hpp"

struct FileFinder {
	BSSimpleArray<const char*> Paths;

	enum LOOKIN_FLAGS : UInt8 {
		SKIP_NONE = 0x0,
		SKIP_ARCHIVE = 0x1,
		SKIP_CWD = 0x2,
		SKIP_PATHS = 0x4,
	};

	static FileFinder* GetSingleton();
	static BSFile* GetFile(const char* apName, NiFile::OpenMode aeMode, int aiSize, ARCHIVE_TYPE aeArchiveType);
	static bool Locate(const char* apName, const char* apFilePath, UInt32 aiFlags, ARCHIVE_TYPE aiArchiveType);
	bool Exist(const char* apName, const char* apFilePath, UInt32 aiFlags, ARCHIVE_TYPE aiArchiveType);
};
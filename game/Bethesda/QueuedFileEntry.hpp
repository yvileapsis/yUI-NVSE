#pragma once

#include "QueuedFile.hpp"
#include "Archive.hpp"

class QueuedFileEntry : public QueuedFile {
public:
	QueuedFileEntry();
	~QueuedFileEntry();

	virtual bool Unk_0B() = 0;

	char*			pFileName;
	BSFileEntry*	pData;

	static void __fastcall LookupFileInBSA(QueuedFileEntry* apThis, void*, ARCHIVE_TYPE_INDEX aeArchiveTypeIndex);
	static bool __fastcall CreateDescription(QueuedFileEntry* apThis, void*, const char* apDest, size_t auiSize, const char* apTaskName);

	ArchiveFile* OpenBSFile(ARCHIVE_TYPE_INDEX aeArchiveTypeIndex, SInt32 aiArchiveType);
};
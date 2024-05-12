#pragma once
#include "BSTCaseInsensitiveStringMap.hpp"
#include "BSSimpleArray.hpp"

class ArchiveFileData;
class ArchiveFileCellData;
class ArchiveFileOwner;
class ArchiveAnalysisFile;
class ArchiveFileGroup;
class ArchiveFileOrderData;
class ArchiveFileRelationData;

class ArchiveAnalyzer {
public:
	BSTCaseInsensitiveStringMap<ArchiveFileData*>	kFileDatas;
	BSSimpleArray<ArchiveFileCellData*>				kFileCellDatas;
	BSTCaseInsensitiveStringMap<ArchiveFileOwner*>	kFileOwners;
	BSSimpleArray<ArchiveAnalysisFile*>				kAnalysisFiles;
	BSSimpleArray<ArchiveFileGroup*>				kFileGroups;
	BSSimpleArray<ArchiveFileOrderData*>			kFileOrderDatas;
	UInt32											uiFileCount;
};

ASSERT_SIZE(ArchiveAnalyzer, 0x6C);


// ==============================================================================

struct ArchiveFileRelationData {
	UInt16 usCount; // ?
	UInt16 usFlags; // ?
};

class ArchiveFileData {
public:
	const char*											pFileName;
	UInt16												usCount;
	UInt16												usCellCount;
	NiTMap<ArchiveFileData*, ArchiveFileRelationData>	kRelationMap;
	void*												pUnk18;
};

ASSERT_SIZE(ArchiveFileData, 0x1C);


// ==============================================================================

class ArchiveFileCellData : public BSSimpleArray<ArchiveFileData*> {
public:
	const char*	pCellName;
};

ASSERT_SIZE(ArchiveFileCellData, 0x14);


// ==============================================================================

class ArchiveFileOwner {
public:
	const char* pOwnerName;
};

ASSERT_SIZE(ArchiveFileOwner, 0x4);


// ==============================================================================

class ArchiveAnalysisFile {
public:
	const char*			pFileName;
	ArchiveFileOwner*	pOwner;
};

ASSERT_SIZE(ArchiveAnalysisFile, 0x8);


// ==============================================================================

class ArchiveFileGroup {
public:
	UInt32				unk0;
	ArchiveFileData**	pFiles;
	SInt32				iFileCount;
	UInt32				unkC;
	SInt32				iGroup;
};

ASSERT_SIZE(ArchiveFileGroup, 0x14);


// ==============================================================================

class ArchiveFileOrderData {
public:
	ArchiveFileData*	pFileData;
	UInt32				unk4[6];
};

ASSERT_SIZE(ArchiveFileOrderData, 0x1C);
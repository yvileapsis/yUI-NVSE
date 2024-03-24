#pragma once

#include "FutBinaryFileC.hpp"
#include "LoadedFile.hpp"

class NiBinaryStream;

class BSFaceGenBinaryFile : public FutBinaryFileC {
public:
	BSFaceGenBinaryFile();
	virtual ~BSFaceGenBinaryFile();

	virtual bool LoadFile(DebugString akPath, bool abUseMiscArchive);

	NiBinaryStream* pBinaryStream;
	LoadedFilePtr	spLoadedFile;
};

ASSERT_SIZE(BSFaceGenBinaryFile, 0x48);
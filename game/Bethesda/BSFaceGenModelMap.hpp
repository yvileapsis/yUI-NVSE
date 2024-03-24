#pragma once

#include "BSTCaseInsensitiveStringMap.hpp"
#include "NiRefObject.hpp"

class BSFaceGenModel;

class BSFaceGenModelMap {
public:
	BSFaceGenModelMap();
	virtual ~BSFaceGenModelMap();

	class Entry : public NiRefObject {
	public:
		NiPointer<BSFaceGenModel>	spModel;
		UInt32						uiTime;
	};

	BSTCaseInsensitiveStringMap<NiPointer<BSFaceGenModelMap::Entry>>	kEntryMap;
	UInt32																uiMaxDataSizes[2];
	bool																bHasFaceGenData;
};

ASSERT_SIZE(BSFaceGenModelMap, 0x24);
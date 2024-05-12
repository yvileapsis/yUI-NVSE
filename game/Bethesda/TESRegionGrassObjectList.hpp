#pragma once

#include "BSSimpleList.hpp"
#include "NiSmartPointer.hpp"
#include "TESRegionGrassObject.hpp"

class TESRegionGrassObjectList {
public:
	virtual void Destroy(bool bFreeMemory);

	BSSimpleList<NiPointer<TESRegionGrassObject>> list;
	bool bOwnsTESRegionGrassObjectMemory;
	UInt32 iCount;
};

ASSERT_SIZE(TESRegionGrassObjectList, 0x14)
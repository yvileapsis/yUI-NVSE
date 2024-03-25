#pragma once
#include "BSSimpleList.hpp"
#include "NiSmartPointer.hpp"
#include "TESRegionGrassObject.hpp"

class TESRegionGrassObjectList {
public:
	virtual void Destroy(bool bFreeMemory);

	BSSimpleList<NiPointer<TESRegionGrassObject>> kList;
	bool	bOwnsTESRegionGrassObjectMemory;
	UInt32	uiCount;
};
static_assert(sizeof(TESRegionGrassObjectList) == 0x14);
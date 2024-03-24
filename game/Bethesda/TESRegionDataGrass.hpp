#pragma once

#include "TESRegionData.hpp"
#include "TESRegionGrassObjectList.hpp"

class TESRegionDataGrass : public TESRegionData {
public:
	TESRegionDataGrass();
	~TESRegionDataGrass();

	TESRegionGrassObjectList* TESRegionGrassObjectList;
};

ASSERT_SIZE(TESRegionDataGrass, 0xC)
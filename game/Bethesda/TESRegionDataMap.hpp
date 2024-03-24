#pragma once

#include "TESRegionData.hpp"
#include "BSString.hpp"

class TESRegionDataMap : public TESRegionData {
public:
	TESRegionDataMap();
	~TESRegionDataMap();

	BSStringT RegionName;	// 08
};

ASSERT_SIZE(TESRegionDataMap, 0x10)
#pragma once
#include "TESRegionData.hpp"
#include "BSString.hpp"

// 0x10
class TESRegionDataMap : public TESRegionData {
public:
	TESRegionDataMap();
	~TESRegionDataMap();

	BSStringT kRegionName;	// 08
};
static_assert(sizeof(TESRegionDataMap) == 0x10);
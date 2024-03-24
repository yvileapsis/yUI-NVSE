#pragma once

#include "TESRegionData.hpp"

class TESObjectREFR;

class TESRegionDataImposter : public TESRegionData {
public:
	TESRegionDataImposter();
	~TESRegionDataImposter();

	BSSimpleList<TESObjectREFR*>	pObjects;	// 08
};

ASSERT_SIZE(TESRegionDataImposter, 0x10)
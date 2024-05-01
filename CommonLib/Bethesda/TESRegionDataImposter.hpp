#pragma once
#include "TESRegionData.hpp"

class TESObjectREFR;

// 0x10
class TESRegionDataImposter : public TESRegionData {
public:
	TESRegionDataImposter();
	~TESRegionDataImposter();

	BSSimpleList<TESObjectREFR*>	pkObjects;	// 08
};
static_assert(sizeof(TESRegionDataImposter) == 0x10);
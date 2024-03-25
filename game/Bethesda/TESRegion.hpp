#pragma once
#include "TESForm.hpp"
#include "NiPoint3.hpp"
#include "NiColor.hpp"
#include "BSSimpleList.hpp"

class TESWorldSpace;
class TESWeather;
class TESRegionData;

// 0x38
class TESRegion : public TESForm
{
public:
	TESRegion();
	~TESRegion() override;

	struct AreaPointEntry
	{
		float	x;
		float	y;
	};

	struct RegionAreaEntry
	{
		BSSimpleList<AreaPointEntry*>	kPoints;
		UInt32							unk08[2];
		Float32							unk10[4];
		UInt32							uiEdgeFallOff;
		UInt32							uiPointCount;
	};
	typedef BSSimpleList<RegionAreaEntry> RegionAreaEntryList;

	BSSimpleList<TESRegionData*>*	pkDataEntries;	// 18
	BSSimpleList<RegionAreaEntry*>*	pkAreaEntries;	// 1C
	TESWorldSpace*					pkWorldSpace;
	TESWeather*						pkWeather;
	Float32							unk28;
	NiColor							kExternalEmittance;
};
static_assert(sizeof(TESRegion) == 0x38);
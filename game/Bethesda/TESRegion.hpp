#pragma once

#include "TESForm.hpp"
#include "TESWorldSpace.hpp"
#include "NiColor.hpp"
#include "NiPoint3.hpp"
#include "BSSimpleList.hpp"
#include "TESRegionData.hpp"

class TESWeather;

class TESRegion : public TESForm {
public:
	TESRegion();
	virtual ~TESRegion();

	typedef BSSimpleList<NiPoint2*> AreaPointEntryList;

	struct RegionAreaEntry {
		AreaPointEntryList	kPoints;
		UInt32				unk08[2];
		float				unk10[4];
		UInt32				edgeFallOff;
		UInt32				uiPointCount;
	};
	typedef BSSimpleList<RegionAreaEntry*> RegionAreaEntryList;

	RegionDataEntryList*	pDataEntries;
	RegionAreaEntryList*	pAreaEntries;
	TESWorldSpace*			pWorldSpace;
	TESWeather*				pWeather;
	float					fUnk28;
	NiColor					kExternalEmittance;
};

ASSERT_SIZE(TESRegion, 0x38);
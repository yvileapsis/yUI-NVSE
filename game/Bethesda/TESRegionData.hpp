#pragma once

#include "BaseFormComponent.hpp"
#include "BSSimpleList.hpp"

class TESRegionData : public BaseFormComponent {
public:
	TESRegionData();
	~TESRegionData();

	bool bOverride;
	bool bIgnore;
	char cPriority;
};
typedef BSSimpleList<TESRegionData> RegionDataEntryList;

ASSERT_SIZE(TESRegionData, 0x8)
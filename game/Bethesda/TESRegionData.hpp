#pragma once
#include "BaseFormComponent.hpp"
#include "BSSimpleList.hpp"

// 0x8
class TESRegionData : public BaseFormComponent {
public:
	TESRegionData();
	~TESRegionData();

	bool bOverride;
	bool bIgnore;
	char cPriority;
};
static_assert(sizeof(TESRegionData) == 0x8);
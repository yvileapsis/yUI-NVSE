#pragma once

#include "TESForm.hpp"
#include "BGSPreloadable.hpp"

class BGSImpactData;

class BGSImpactDataSet : public TESForm, public BGSPreloadable {
public:
	BGSImpactDataSet();
	virtual ~BGSImpactDataSet();

	BGSImpactData* impactDatas[12];
};

ASSERT_SIZE(BGSImpactDataSet, 0x4C);
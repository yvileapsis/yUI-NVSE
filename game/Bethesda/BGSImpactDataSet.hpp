#pragma once
#include "TESForm.hpp"
#include "BGSPreloadable.hpp"

// 0x4C
class BGSImpactDataSet :
	public TESForm,
	public BGSPreloadable
{
public:
	BGSImpactDataSet();
	virtual ~BGSImpactDataSet();

	BGSImpactData* impactDatas[12];
};
static_assert(sizeof(BGSImpactDataSet) == 0x4C);
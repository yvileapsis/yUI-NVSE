#pragma once
#include "TESPackageData.hpp"

class TESPatrolPackageData : public TESPackageData
{
public:
	TESPatrolPackageData();
	~TESPatrolPackageData();

	UInt8	patrolFlags;
};
#pragma once
#include "TESPackage.hpp"

// 0x88
class AlarmPackage : public TESPackage
{
	BSSimpleList<void*> unklist80;
};
static_assert(sizeof(AlarmPackage) == 0x88);
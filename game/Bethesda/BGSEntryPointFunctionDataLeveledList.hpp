#pragma once
#include "BGSEntryPointFunctionData.hpp"

class TESLevItem;

// 0x8
class BGSEntryPointFunctionDataLeveledList : public BGSEntryPointFunctionData 
{
public:
	BGSEntryPointFunctionDataLeveledList();
	~BGSEntryPointFunctionDataLeveledList();

	TESLevItem* pkLeveledList;		// 04
};
static_assert(sizeof(BGSEntryPointFunctionDataLeveledList) == 0x8);
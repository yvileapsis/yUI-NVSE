#pragma once

#include "BGSEntryPointFunctionData.hpp"

class TESLevItem;

class BGSEntryPointFunctionDataLeveledList : public BGSEntryPointFunctionData {
public:
	BGSEntryPointFunctionDataLeveledList();
	~BGSEntryPointFunctionDataLeveledList();

	TESLevItem* leveledList;		// 04
};
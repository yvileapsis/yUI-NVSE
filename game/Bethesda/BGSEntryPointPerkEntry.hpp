#pragma once

#include "BGSPerkEntry.hpp"
#include "TESCondition.hpp"

class BGSEntryPointFunctionData;

struct EntryPointConditions {
	TESCondition		tab1;
	TESCondition		tab2;
	TESCondition		tab3;
};

// 14
class BGSEntryPointPerkEntry : public BGSPerkEntry {
public:
	BGSEntryPointPerkEntry();
	~BGSEntryPointPerkEntry();

	UInt8						entryPoint;		// 08
	UInt8						function;		// 09
	UInt8						conditionTabs;	// 0A
	UInt8						pad0B;			// 0B
	BGSEntryPointFunctionData* data;			// 0C
	EntryPointConditions* conditions;	// 10
};
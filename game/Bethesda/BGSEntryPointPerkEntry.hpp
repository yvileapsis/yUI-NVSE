#pragma once
#include "BGSPerkEntry.hpp"
#include "TESCondition.hpp"

class BGSEntryPointFunctionData;

// 14
class BGSEntryPointPerkEntry : public BGSPerkEntry 
{
public:
	BGSEntryPointPerkEntry();
	~BGSEntryPointPerkEntry();

	struct EntryPointConditions {
		TESCondition		tab1;
		TESCondition		tab2;
		TESCondition		tab3;
	};

	UInt8						ucEntryPoint;		// 08
	UInt8						ucFunction;			// 09
	UInt8						ucConditionTabs;	// 0A
	UInt8						pad0B;				// 0B
	BGSEntryPointFunctionData*	pkData;				// 0C
	EntryPointConditions*		pkConditions;		// 10
};
static_assert(sizeof(BGSEntryPointPerkEntry) == 0x14);
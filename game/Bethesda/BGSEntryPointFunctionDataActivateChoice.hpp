#pragma once
#include "BGSEntryPointFunctionData.hpp"
#include "BSString.hpp"

// 0x14
class BGSEntryPointFunctionDataActivateChoice : public BGSEntryPointFunctionData {
public:
	BGSEntryPointFunctionDataActivateChoice();
	~BGSEntryPointFunctionDataActivateChoice();

	BSStringT			kLabel;				// 04
	Script*				pkScript;			// 0C
	UInt32				uiFlags;			// 10
};
static_assert(sizeof(BGSEntryPointFunctionDataActivateChoice) == 0x14);
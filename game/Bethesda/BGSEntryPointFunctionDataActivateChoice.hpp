#pragma once

#include "BGSEntryPointFunctionData.hpp"
#include "BSString.hpp"

class BGSEntryPointFunctionDataActivateChoice : public BGSEntryPointFunctionData {
public:
	BGSEntryPointFunctionDataActivateChoice();
	~BGSEntryPointFunctionDataActivateChoice();

	BSStringT			label;				// 04
	Script* script;			// 0C
	UInt32				flags;				// 10
};
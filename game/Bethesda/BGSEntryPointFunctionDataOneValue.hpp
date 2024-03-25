#pragma once
#include "BGSEntryPointFunctionData.hpp"

// 0x8
class BGSEntryPointFunctionDataOneValue : public BGSEntryPointFunctionData
{
public:
	BGSEntryPointFunctionDataOneValue();
	~BGSEntryPointFunctionDataOneValue();

	Float32				value;				// 04
};
static_assert(sizeof(BGSEntryPointFunctionDataOneValue) == 0x8);

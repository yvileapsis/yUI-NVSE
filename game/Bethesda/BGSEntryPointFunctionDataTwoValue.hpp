#pragma once
#include "BGSEntryPointFunctionData.hpp"

// 0C
class BGSEntryPointFunctionDataTwoValue : public BGSEntryPointFunctionData 
{
public:
	BGSEntryPointFunctionDataTwoValue();
	~BGSEntryPointFunctionDataTwoValue();

	Float32				value[2];			// 04
};
static_assert(sizeof(BGSEntryPointFunctionDataTwoValue) == 0xC);
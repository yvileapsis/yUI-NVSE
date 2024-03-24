#pragma once

#include "BGSEntryPointFunctionData.hpp"

class BGSEntryPointFunctionDataTwoValue : public BGSEntryPointFunctionData {
public:
	BGSEntryPointFunctionDataTwoValue();
	~BGSEntryPointFunctionDataTwoValue();

	float				value[2];			// 04
};
#pragma once

#include "BGSEntryPointFunctionData.hpp"

class BGSEntryPointFunctionDataOneValue : public BGSEntryPointFunctionData {
public:
	BGSEntryPointFunctionDataOneValue();
	~BGSEntryPointFunctionDataOneValue();

	float				value;				// 04
};
#pragma once
#include "BSExtraData.hpp"

class ExtraHotkey : public BSExtraData {
public:
	ExtraHotkey();
	virtual ~ExtraHotkey();

	UInt8	index;		// 00C (is 0-7)

	static ExtraHotkey* Create();
};
static_assert(sizeof(ExtraHotkey) == 0x10);
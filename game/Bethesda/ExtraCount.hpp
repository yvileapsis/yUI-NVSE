#pragma once
#include "BSExtraData.hpp"

class ExtraCount : public BSExtraData {
public:
	ExtraCount();
	virtual ~ExtraCount();

	SInt16	count;	// 00C
	UInt8	pad[2];	// 00E

	static ExtraCount* Create(UInt32 count = 0);
};
static_assert(sizeof(ExtraCount) == 0x10);
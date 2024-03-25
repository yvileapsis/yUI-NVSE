#pragma once
#include "BSExtraData.hpp"

class ExtraSeenData : public BSExtraData {
public:
	ExtraSeenData();
	virtual ~ExtraSeenData();

	UInt8 unk[0x24 - 0x0C];		// 00C
};
static_assert(sizeof(ExtraSeenData) == 0x24);
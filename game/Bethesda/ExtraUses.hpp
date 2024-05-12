#pragma once
#include "BSExtraData.hpp"

class ExtraUses : public BSExtraData {
public:
	ExtraUses();
	~ExtraUses();

	UInt32 unk0;

	static ExtraUses* Create();
};
static_assert(sizeof(ExtraUses) == 0x10);
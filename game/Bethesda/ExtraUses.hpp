#pragma once

#include "BSExtraData.hpp"

class ExtraUses : public BSExtraData {
public:
	ExtraUses();
	~ExtraUses();

	UInt32 unk0;

	static ExtraUses* Create();
};
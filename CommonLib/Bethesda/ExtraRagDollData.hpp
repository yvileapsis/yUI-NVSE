#pragma once

#include "BSExtraData.hpp"
#include "RagDollData.hpp"

class ExtraRagDollData : public BSExtraData {
public:
	ExtraRagDollData();
	virtual ~ExtraRagDollData();

	RagDollData* pData;
};

ASSERT_SIZE(ExtraRagDollData, 0x10);
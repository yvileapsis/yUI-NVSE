#pragma once

#include "BSExtraData.hpp"

class ExtraRank : public BSExtraData {
public:
	ExtraRank();
	virtual ~ExtraRank();

	UInt32	rank; // 00C

	static ExtraRank* Create();
};
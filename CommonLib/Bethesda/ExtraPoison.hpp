#pragma once
#include "BSExtraData.hpp"

class ExtraPoison : public BSExtraData
{
public:
	ExtraPoison();
	virtual ~ExtraPoison();

	AlchemyItem* poisonEffect;	// 0C
};
static_assert(sizeof(ExtraPoison) == 0x10);
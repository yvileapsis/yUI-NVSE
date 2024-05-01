#pragma once
#include "BSExtraData.hpp"

class TESGlobal;

// 0x10
class ExtraGlobal : public BSExtraData {								//ownership data, stored separately from ExtraOwnership
public:
	ExtraGlobal();
	virtual ~ExtraGlobal();

	TESGlobal* globalVar;	// 00C
};
static_assert(sizeof(ExtraGlobal) == 0x10);
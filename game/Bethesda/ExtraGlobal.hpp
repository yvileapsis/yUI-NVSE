#pragma once

#include "BSExtraData.hpp"

class TESGlobal;

class ExtraGlobal : public BSExtraData {								//ownership data, stored separately from ExtraOwnership
public:
	ExtraGlobal();
	virtual ~ExtraGlobal();

	TESGlobal* globalVar;	// 00C
};
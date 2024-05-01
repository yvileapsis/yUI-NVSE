#pragma once
#include "TESModel.hpp"

// 0x18
class TESModelRDT : public TESModel {
public:
	TESModelRDT();
	~TESModelRDT();

	virtual UInt32	Fn_07();
};
static_assert(sizeof(TESModelRDT) == 0x18);
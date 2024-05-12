#pragma once

#include "TESModel.hpp"

class TESModelRDT : public TESModel {
public:
	TESModelRDT();
	~TESModelRDT();

	virtual UInt32	Fn_07(void);
};
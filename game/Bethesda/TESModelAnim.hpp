#pragma once

#include "TESModel.hpp"

class TESModelAnim : public TESModel {
public:
	TESModelAnim();		// Identical to TESModel with a different vTable
	~TESModelAnim();
};	// 018
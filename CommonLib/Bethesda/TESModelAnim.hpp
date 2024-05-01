#pragma once
#include "TESModel.hpp"

// 0x18
class TESModelAnim : public TESModel
{
public:
	TESModelAnim();		// Identical to TESModel with a different vTable
	~TESModelAnim();
};
static_assert(sizeof(TESModelAnim) == 0x18);
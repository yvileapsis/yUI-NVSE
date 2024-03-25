#pragma once
#include "TESForm.hpp"

class SpellItem;

// 0x20
class BGSRadiationStage : public TESForm 
{
public:
	BGSRadiationStage();
	~BGSRadiationStage();

	UInt32		uiThreshold;
	SpellItem*	pkEffect;
};
static_assert(sizeof(BGSRadiationStage) == 0x20);
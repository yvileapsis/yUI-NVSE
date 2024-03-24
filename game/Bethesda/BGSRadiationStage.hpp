#pragma once

#include "TESForm.hpp"

class SpellItem;

class BGSRadiationStage : public TESForm {
public:
	BGSRadiationStage();
	~BGSRadiationStage();

	UInt32 threshold;
	SpellItem* effect;
};
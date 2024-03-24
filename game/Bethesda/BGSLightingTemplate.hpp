#pragma once

#include "TESObjectCELL.hpp"

class BGSLightingTemplate : public TESForm {
public:
	BGSLightingTemplate();
	~BGSLightingTemplate();

	TESObjectCELL::InteriorData Data;
};

ASSERT_SIZE(BGSLightingTemplate, 0x44);
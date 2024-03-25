#pragma once
#include "TESObjectCELL.hpp"

class BGSLightingTemplate :
	public TESForm,							// 00
	public TESObjectCELL::InteriorData		// 18
{
public:
	BGSLightingTemplate();
	~BGSLightingTemplate();
};
static_assert(sizeof(BGSLightingTemplate) == 0x44);
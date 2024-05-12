#pragma once
#include "TESModel.hpp"

// 0x18
class BGSTextureModel : public TESModel
{
public:
	BGSTextureModel();
	~BGSTextureModel() override;
};
static_assert(sizeof(TESModel) == 0x18);
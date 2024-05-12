#pragma once
#include "TESTexture.hpp"
// Complete

// 0xC
class TESIcon : public TESTexture {
public:
	TESIcon();
	~TESIcon();
};
static_assert(sizeof(TESIcon) == 0xC);
#pragma once

#include "TESBoundObject.hpp"
#include "TESModelTextureSwap.hpp"

class BGSStaticCollection : public TESBoundObject {
public:
	TESModelTextureSwap modelSwap;
};

ASSERT_SIZE(BGSStaticCollection, 0x50);
#pragma once

#include "TESBoundObject.hpp"
#include "TESModelTextureSwap.hpp"

class TESObjectSTAT : public TESBoundObject, public TESModelTextureSwap {
public:
	TESObjectSTAT();
	~TESObjectSTAT();

	// children
	UInt32				unk50[2];
};

ASSERT_SIZE(TESObjectSTAT, 0x58);
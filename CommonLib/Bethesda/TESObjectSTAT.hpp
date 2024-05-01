#pragma once
#include "TESBoundObject.hpp"
#include "TESModelTextureSwap.hpp"

// 0x58
class TESObjectSTAT :
	public TESBoundObject,
	public TESModelTextureSwap
{
public:
	TESObjectSTAT();
	~TESObjectSTAT();

	// children
	UInt32 unk50[2];
};
static_assert(sizeof(TESObjectSTAT) == 0x58);
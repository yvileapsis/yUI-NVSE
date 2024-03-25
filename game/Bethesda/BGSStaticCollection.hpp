#pragma once
#include "TESBoundObject.hpp"
#include "TESModelTextureSwap.hpp"

// 0x50
class BGSStaticCollection :
	public TESBoundObject, 
	public TESModelTextureSwap
{
public:
};
static_assert(sizeof(BGSStaticCollection) == 0x50);
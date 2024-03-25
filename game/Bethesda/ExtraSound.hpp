#pragma once
#include "BSExtraData.hpp"
#include "BSSoundHandle.hpp"

class ExtraSound :
	public BSExtraData,
	public BSSoundHandle
{
public:
	ExtraSound();
	~ExtraSound();

};
static_assert(sizeof(ExtraSound) == 0x18);
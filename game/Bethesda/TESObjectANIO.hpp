#pragma once
#include "TESForm.hpp"
#include "TESModelTextureSwap.hpp"

// 0x3C
class TESObjectANIO :
	public TESForm,
	public TESModelTextureSwap 
{
public:
	TESObjectANIO();
	~TESObjectANIO();

	UInt32	unk38;						// 38
};
static_assert(sizeof(TESObjectANIO) == 0x3C);
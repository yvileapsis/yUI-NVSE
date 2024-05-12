#pragma once

#include "TESForm.hpp"
#include "TESModelTextureSwap.hpp"

class TESObjectANIO : public TESForm {
public:
	TESObjectANIO();
	~TESObjectANIO();

	TESModelTextureSwap	modelSwap;		// 18
	UInt32	unk38;						// 38
};
#pragma once
#include "BGSPrimitive.hpp"

// 0x4C
class BGSPrimitiveBox : public BGSPrimitive {
public:
	static void __fastcall BuildEx(BGSPrimitiveBox* apThis);
	UInt32			unk34;
	UInt32			unk38;
	UInt32			unk3C;
	UInt32			unk40;
	UInt32			unk44;
	UInt32			unk48;
};
static_assert(sizeof(BGSPrimitiveBox) == 0x4C);
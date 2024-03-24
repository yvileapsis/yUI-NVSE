#pragma once

#include "TESObjectACTI.hpp"

class TESFurniture : public TESObjectACTI {
	UInt32 unk;
};

ASSERT_SIZE(TESFurniture, 0x94);
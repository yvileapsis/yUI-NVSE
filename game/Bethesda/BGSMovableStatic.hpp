#pragma once

#include "BGSDestructibleObjectForm.hpp"
#include "TESFullName.hpp"
#include "TESObjectSTAT.hpp"

class BGSMovableStatic {
public:
	TESFullName name;
	BGSDestructibleObjectForm destructibleObject;
	TESObjectSTAT statObject;
	UInt32 gap6C[2];
};

ASSERT_SIZE(BGSMovableStatic, 0x74);
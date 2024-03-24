#pragma once

#include "TESBoundObject.hpp"
#include "TESModelTextureSwap.hpp"
#include "TESLeveledList.hpp"

class TESLevCreature : public TESBoundObject {
public:
	TESLevCreature();
	~TESLevCreature();

	TESLeveledList		list;		// 030
	TESModelTextureSwap	texture;	// 04C
	UInt32 unk6C;
};

ASSERT_SIZE(TESLevCreature, 0x70);
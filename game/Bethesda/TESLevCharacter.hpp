#pragma once

#include "TESBoundObject.hpp"
#include "TESModelTextureSwap.hpp"
#include "TESLeveledList.hpp"

class TESLevCharacter : public TESBoundObject {
public:
	TESLevCharacter();
	~TESLevCharacter();

	TESLeveledList		list;		// 030
	TESModelTextureSwap	texture;	// 04C
	UInt32 unk6C;
};

ASSERT_SIZE(TESLevCharacter, 0x70);
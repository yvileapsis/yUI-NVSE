#pragma once
#include "TESBoundObject.hpp"
#include "TESModelTextureSwap.hpp"
#include "TESLeveledList.hpp"

// 0x70
class TESLevCharacter :
	public TESBoundObject,		// 000
	public TESLeveledList, 		// 030
	public TESModelTextureSwap	// 04C
{
public:
	TESLevCharacter();
	~TESLevCharacter();

	UInt32 unk6C;
};
static_assert(sizeof(TESLevCharacter) == 0x70);
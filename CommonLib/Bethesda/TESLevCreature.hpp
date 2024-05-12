#pragma once
#include "TESBoundObject.hpp"
#include "TESModelTextureSwap.hpp"
#include "TESLeveledList.hpp"

// 0x70
class TESLevCreature :
	public TESBoundObject,		// 000
	public TESLeveledList, 		// 030
	public TESModelTextureSwap	// 04C
{
public:
	TESLevCreature();
	~TESLevCreature();

	UInt32 unk6C;
};
static_assert(sizeof(TESLevCreature) == 0x70);
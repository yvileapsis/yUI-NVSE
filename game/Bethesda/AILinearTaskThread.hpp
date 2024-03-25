#pragma once
#include "BSLinearTaskThread.hpp"

// 0x78
class AILinearTaskThread : public BSLinearTaskThread {
public:
	void(__cdecl* function)();
	UInt32 memRouter; // hkMemoryRouterx
	UInt32 unk54;
	UInt32 unk58;
	UInt32 unk5C;
	UInt32 unk60;
	UInt32 unk64;
	UInt32 unk68;
	UInt32 unk6C;
	UInt32 unk70;
	UInt32 unk74;
};
static_assert(sizeof(AILinearTaskThread) == 0x78);
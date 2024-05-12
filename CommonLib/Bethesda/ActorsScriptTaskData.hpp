#pragma once
#include "MobileObjectTaskletData.hpp"

class Actor;

// 0x40
class ActorsScriptTaskData : public MobileObjectTaskletData {
public:
	UInt32 unk38;
	UInt32 uiRunCount;

	static void __fastcall ExecuteTaskEx(ActorsScriptTaskData* apThis, void*, Actor* apActor);
};
static_assert(sizeof(ActorsScriptTaskData) == 0x40);
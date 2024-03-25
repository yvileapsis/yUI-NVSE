#pragma once
#include "MobileObjectTaskletData.hpp"

class Actor;

// 0x38 Unused
class AnimationTaskData : public MobileObjectTaskletData {
public:

	static void __fastcall ExecuteTaskEx(AnimationTaskData* apThis, void*, Actor* apActor);
};
static_assert(sizeof(AnimationTaskData) == 0x38);
#pragma once

#include "MobileObjectTaskletData.hpp"

class Actor;

// Unused
class AnimationTaskData : public MobileObjectTaskletData {
public:

	static void __fastcall ExecuteTaskEx(AnimationTaskData* apThis, void*, Actor* apActor);
};

ASSERT_SIZE(AnimationTaskData, 0x38);
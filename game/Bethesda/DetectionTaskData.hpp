#pragma once

#include "MobileObjectTaskletData.hpp"

class Actor;

// Unused
class DetectionTaskData : public MobileObjectTaskletData {
public:
	float flt38;
	UInt32 uiRunCount;

	static void __fastcall ExecuteTaskEx(DetectionTaskData* apThis, void*, Actor* apActor);
};

ASSERT_SIZE(DetectionTaskData, 0x40);
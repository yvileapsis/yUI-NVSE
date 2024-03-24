#pragma once

#include "MobileObjectTaskletData.hpp"

class TESObjectREFR;

// Unused
class MovementTaskData : public MobileObjectTaskletData {
public:
	UInt32 uiRunCount;

	static void __fastcall ExecuteTaskEx(MovementTaskData* apThis, void*, TESObjectREFR* apRefr);
};

ASSERT_SIZE(MovementTaskData, 0x3C);
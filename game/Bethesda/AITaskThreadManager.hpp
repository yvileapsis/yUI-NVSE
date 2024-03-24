#pragma once

#include "AITaskThread.hpp"

struct AITaskThreadManager {
	AITaskThread* pThreads[3];
	UInt32 uiTaskCount;
};

ASSERT_SIZE(AITaskThreadManager, 0x10);
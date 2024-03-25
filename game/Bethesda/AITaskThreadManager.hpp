#pragma once
#include "AITaskThread.hpp"

// 0x10
struct AITaskThreadManager {
	AITaskThread*	pThreads[3];
	UInt32			uiTaskCount;
};
static_assert(sizeof(AITaskThreadManager) == 0x10);
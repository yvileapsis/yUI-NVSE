#pragma once
#include "AILinearTaskThread.hpp"

// 0x94
struct AILinearTaskManager {
	AILinearTaskThread*	pThreads[3];
	UInt32				unkC[3];
	BSSemaphore			semaphores[3];
	HANDLE*				eventsArray3C[12];
	HANDLE*				eventsArray6C[8];
	UInt32				unk8C;
	UInt32				unk90;
};
static_assert(sizeof(AILinearTaskManager) == 0x94);
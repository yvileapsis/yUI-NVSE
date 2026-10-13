#pragma once
#include "AILinearTaskThread.hpp"

// 0x94
struct AILinearTaskThreadManager {
	AILinearTaskThread* pThreads[3];
	uint32_t			eThreadStage[3];
	BSSemaphore			kSemaphores[3];
	HANDLE*				pFirstThreadEvents[12]; // First is either thread 0, or 1
	HANDLE*				pSecondThreadEvents[8]; // Always thread 2
	HANDLE*				pEvents[2];

	static AILinearTaskThreadManager* GetSingleton();
	static bool IsRunningThreads();
	static bool IsMainRendering();
	static bool ShouldQueue3DTask();
};
static_assert(sizeof(AILinearTaskThreadManager) == 0x94);
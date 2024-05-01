#pragma once
#include "BSPackedTask.hpp"
#include "BSLinearTaskThread.hpp"
#include "BSScrapHeapQueueElem.hpp"

// 0x78
class AITaskThread : public BSLinearTaskThread {
public:
	DWORD dword4C;
	BSScrapHeapQueueElem<BSPackedTask> dword50;
	DWORD unk74;
};
static_assert(sizeof(AITaskThread) == 0x78);
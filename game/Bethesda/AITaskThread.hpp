#pragma once

#include "BSPackedTask.hpp"
#include "BSLinearTaskThread.hpp"
#include "BSScrapHeapQueueElem.hpp"

class AITaskThread : public BSLinearTaskThread {
public:
	DWORD dword4C;
	BSScrapHeapQueueElem<BSPackedTask> dword50;
	DWORD unk74;
};

ASSERT_SIZE(AITaskThread, 0x78);
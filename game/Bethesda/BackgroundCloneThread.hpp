#pragma once

#include "BSTaskThread.hpp"
#include "LockFreeQueue.hpp"
#include "NiSmartPointer.hpp"
#include "QueuedReference.hpp"

class BackgroundCloneThread : public BSTaskThread {
public:
	bool										bExit;
	SInt32										iRunningCount;
	LockFreeQueue<NiPointer<QueuedReference>>*	pProcessTaskQueue;
};

ASSERT_SIZE(BackgroundCloneThread, 0x3C);
#pragma once
#include "BSTaskThread.hpp"
#include "LockFreeQueue.hpp"
#include "NiSmartPointer.hpp"
#include "QueuedReference.hpp"

// 0x3C
class BackgroundCloneThread : public BSTaskThread {
public:
	bool	bExit;
	SInt32	iRunningCount;
	LockFreeQueue<NiPointer<QueuedReference>>*	pkProcessTaskQueue;
};
static_assert(sizeof(BackgroundCloneThread) == 0x3C);
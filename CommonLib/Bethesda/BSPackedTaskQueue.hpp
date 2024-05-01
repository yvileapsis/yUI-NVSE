#pragma once

#include <InterfacedClass.hpp>
#include "BSTCommonScrapHeapMessageQueue.hpp"
#include "BSPackedTask.hpp"

class BSPackedTaskQueue {
public:
	BSTCommonScrapHeapMessageQueue<BSPackedTask> Queue;
	void(__cdecl* pUnpackFunc)(BSPackedTask*);
	bool bProcessing;

	bool AddTask(BSPackedTask& akTask);
};

ASSERT_SIZE(BSPackedTaskQueue, 0x28);
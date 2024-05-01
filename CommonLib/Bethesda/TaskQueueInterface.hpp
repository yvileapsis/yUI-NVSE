#pragma once

#include "BSPackedTaskQueue.hpp"
#include "BSPackedTask.hpp"

class NiRefObject;

struct TaskQueueInterface {
public:
	BSPackedTaskQueue*	pQueue;
	BSPackedTaskQueue*	pSecondaryQueue;
	BSPackedTaskQueue*	pActiveQueue;
	UInt32				threadID0C;

	static bool* const bIsActive;

	static TaskQueueInterface* GetSingleton();

	void QueueIfActive(BSPackedTask& akTask);
	static void __fastcall QueueIfActiveEx(TaskQueueInterface* apThis, void*, BSPackedTask& akTask);

	void QueueUpdate3D(NiRefObject* apObject);

	void TaskUnpackFunc(BSPackedTask& akTask);


	void CellTransformsUpdateAddNode(NiRefObject* apObject);
	void QueueCellTransformsUpdate();
};

ASSERT_SIZE(TaskQueueInterface, 0x10);
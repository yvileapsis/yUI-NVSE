#pragma once

#include "BSTask.hpp"

enum IO_TASK_PRIORITY {
	IO_TASK_PRIORITY_CRITICAL	= 0,
	IO_TASK_PRIORITY_VERY_HIGH	= 1,
	IO_TASK_PRIORITY_HIGH		= 2,
	IO_TASK_PRIORITY_MID		= 3,
	IO_TASK_PRIORITY_LOW		= 4,
	IO_TASK_PRIORITY_VERY_LOW	= 5,
	IO_TASK_PRIORITY_COUNT		= 6,
	IO_TASK_PRIORITY_INVALID	= 7,
	IO_TASK_PRIORITY_HIGHEST	= IO_TASK_PRIORITY_CRITICAL,
	IO_TASK_PRIORITY_LOWEST		= IO_TASK_PRIORITY_VERY_LOW,
};

class IOTask : public BSTask<SInt64> {
public:
	IOTask();
	virtual ~IOTask();

	virtual void PostProcess();
	virtual void GenerateKey();
	virtual void Requeue(IO_TASK_PRIORITY aeNewPriority);

	bool CompareState(BS_TASK_STATE Comperand, BS_TASK_STATE Exchange);
};

ASSERT_SIZE(IOTask, 0x18);
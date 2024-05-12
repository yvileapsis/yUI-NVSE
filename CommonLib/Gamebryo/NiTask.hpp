#pragma once

#include "NiObject.hpp"

NiSmartPointer(NiTask);

class NiTask : public NiObject {
public:
	NiTask();
	virtual ~NiTask();

	enum TaskStatus
	{
		IDLE = 0,
		PENDING,
		RUNNING,
		COMPLETED,
		ABORTED
	};

	TaskStatus m_eStatus;
};

ASSERT_SIZE(NiTask, 0xC)
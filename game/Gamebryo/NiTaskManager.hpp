#pragma once

#include "NiObject.hpp"
#include "NiCriticalSection.hpp"
#include "NiTQueue.hpp"

class NiTask;

class NiTaskManager : public NiObject
{
public:
	enum TaskPriority
	{
		LOW = 0,
		MEDIUM,
		HIGH,
		IMMEDIATE,
		NUM_PRIORITIES
	};

	enum ManagerStatus
	{
		STOPPED,
		STOPPING,
		RUNNING
	};

	NiTPrimitiveQueue<NiTask*> m_akTaskQueues[NUM_PRIORITIES];
	NiCriticalSection m_kManagerLock;
	ManagerStatus m_eStatus;
};
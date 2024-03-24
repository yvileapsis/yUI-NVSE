#pragma once

#include "BSThread.hpp"
#include "BSSemaphore.hpp"

class BSLinearTaskThread : public BSThread {
public:
	virtual void OnStartup() {};
	virtual void OnShutdown() {};
	virtual void RunLinearTasks() = 0;

	bool ThreadProc() override;

	BSLinearTaskThread();
	~BSLinearTaskThread();

	BSSemaphore StartProcessing;
	BSSemaphore FinishedProcessing;
	bool		bKillThread;
};

ASSERT_SIZE(BSLinearTaskThread, 0x4C);
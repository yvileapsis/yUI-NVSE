#pragma once

#include "BSSemaphore.hpp"
#include "MiscUtils.hpp"

class BSTaskThread {
public:
	BSTaskThread(const char* apThreadName);
	virtual ~BSTaskThread();
	virtual void RunTasks();

	HANDLE		hThread;
	DWORD		uiThreadID;
	BSSemaphore semaphore1;
	BSSemaphore semaphore2;
	void*		pHavokMemoryRouter;
	DWORD		dword28;
	DWORD		dword2C;
};

ASSERT_SIZE(BSTaskThread, 0x30);
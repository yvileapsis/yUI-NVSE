#pragma once

#include "BSSemaphore.hpp"
#include "MiscUtils.hpp"

class BSTaskThread {
public:
	BSTaskThread(const char* apThreadName);
	virtual ~BSTaskThread();
	virtual void RunTasks() = 0;

	HANDLE		hThread;
	DWORD		uiThreadID;
	BSSemaphore semaphore1;
	BSSemaphore semaphore2;
	void*		pHavokMemoryRouter;
	DWORD		dword28;
	DWORD		dword2C;

	static BSTaskThread* __fastcall CreateThreadEx(BSTaskThread* apThis, void*, void*, const char* apName) {
		ThisStdCall(0xC42DD0, apThis, 0, apName);
		SetThreadName(apThis->hThread, apName);
		return apThis;
	}

	static DWORD WINAPI ThreadProc(LPVOID lpThreadParameter);
};

ASSERT_SIZE(BSTaskThread, 0x30);
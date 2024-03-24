#pragma once

class BSSemaphore {
public:
	BSSemaphore();
	~BSSemaphore();

	UInt32 uiCount;
	HANDLE hSemaphore;
	UInt32 uiMaxCount;

	void Lock();
	void Wait();
};
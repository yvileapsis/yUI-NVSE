#pragma once

#include "BSThread.hpp"
#include "BSSemaphore.hpp"

class LoadingMenuThread : public BSThread {
public:
	BSSemaphore semaphore030;
	BSSemaphore semaphore03C;
	bool		bInactive;
	bool		byte049;
	bool		bExit;
	bool		gap04B;

	static LoadingMenuThread* GetSingleton() { return *(LoadingMenuThread**)0x11DA0C4; }
};

ASSERT_SIZE(LoadingMenuThread, 0x4C);
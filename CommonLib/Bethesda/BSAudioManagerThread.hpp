#pragma once

#include "BSThread.hpp"
#include "BSSemaphore.hpp"

class BSAudioManagerThread : public BSThread {
public:
	BSAudioManagerThread();
	virtual ~BSAudioManagerThread();

	BSSemaphore SuspendSema;
	BSSemaphore SuspendedSema;
	bool		bSuspended;
};	
ASSERT_SIZE(BSAudioManagerThread, 0x4C);
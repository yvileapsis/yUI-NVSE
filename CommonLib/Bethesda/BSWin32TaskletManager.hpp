#pragma once

#include "BSTaskletManager.hpp"
#include "BSWin32TaskletData.hpp"

class BSWin32TaskletManager : public BSTaskletManager {
public:
	HANDLE							hInitThreadHandle;
	UInt32							uiNumThreads;
	UInt32							uiNumThreadsCreated;
	HANDLE							hThreadHandles[6];
	UInt32							uiThreadIDs[6];
	bool							bTerminated;
	CRITICAL_SECTION				kQueueLock;
	BSWin32TaskletData*				pQueueHead[64];
	BSWin32TaskletData**			ppQueueTail[64];
	UInt32							uiInitialCount;
	HANDLE							hStartSemaphore;
	UInt32							uiMaxCount;

	static BSWin32TaskletManager* GetSingleton();
};

ASSERT_SIZE(BSWin32TaskletManager, 0x278);
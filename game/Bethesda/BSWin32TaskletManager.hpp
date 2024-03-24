#pragma once

#include "BSTaskletManager.hpp"
#include "BSWin32TaskletData.hpp"

class BSWin32TaskletManager : public BSTaskletManager {
public:
	HANDLE hInitThreadHandle;
	UInt32 uiNumThreads;
	UInt32 uiNumThreadsCreated;
	HANDLE hThreadHandles[6];
	UInt32 dwThreadIDs[6];
	bool bTerminated;
	struct _RTL_CRITICAL_SECTION QueueLock;
	BSWin32TaskletData* pQueueHead[64];
	BSWin32TaskletData** ppQueueTail[64];
	LONG lInitialCount;
	HANDLE StartSemaphore;
	LONG lMaxCount;

	static BSWin32TaskletManager* GetSingleton();

	void NameTasklets();

};
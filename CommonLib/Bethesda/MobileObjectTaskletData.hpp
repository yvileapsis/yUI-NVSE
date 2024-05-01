#pragma once

#include "BSWin32TaskletData.hpp"
#include "BSTCommonLLMessageQueue.hpp"

// Unused
class MobileObjectTaskletData : public BSWin32TaskletData {
public:
	virtual void EnterCriticalSections(unsigned int);
	virtual void ExecuteTask(void*);
	virtual void LeaveCriticalSections(unsigned int);

	DWORD dword18;
	BSTCommonLLMessageQueue<UInt32> MessageQueue;
	DWORD dword30;
	DWORD dword34;
};
static_assert(sizeof(MobileObjectTaskletData) == 0x38);
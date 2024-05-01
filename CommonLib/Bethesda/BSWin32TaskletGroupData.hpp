#pragma once

#include "BSTaskletGroupData.hpp"

class BSWin32TaskletGroupData : public BSTaskletGroupData {
public:
	bool bComplete;
	LONG dword8;
	DWORD CompletionSemaphore;
	DWORD dword10;
	_RTL_CRITICAL_SECTION GroupDataLock;
	bool bAttached;
	bool bAllowTaskAttach;
	bool bCancelled;
	UInt32 uiPriority;
	UInt32 uiAttached;
	UInt32 uiCompleted;
};

ASSERT_SIZE(BSWin32TaskletGroupData, 0x3C);
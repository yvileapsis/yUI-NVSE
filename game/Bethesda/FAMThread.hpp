#pragma once

#include "BSThread.hpp"
#include "BSSimpleList.hpp"

class FAMThread : public BSThread {
public:
	HANDLE				hEvent;
	BSSimpleList<void*> kList;
	Bitfield32			uiFlags;
	DWORD				dword40;
	DWORD				dword44;
	UInt32				uiOwnerThreadID;
};

ASSERT_SIZE(FAMThread, 0x4C);
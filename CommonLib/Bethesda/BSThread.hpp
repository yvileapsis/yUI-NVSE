#pragma once

#include "InterfacedClass.hpp"
#include "BSMemObject.hpp"
#include "MiscUtils.hpp"

class BSThread : public BSMemObject {
public:
	BSThread();
	virtual		 ~BSThread();
	virtual bool ThreadProc() { return false; };

	CRITICAL_SECTION	kCriticalSection;
	HANDLE				hCreatedThread;
	HANDLE				hCreatorThread;
	UInt32				uiThreadID;
	UInt32				uiCreatorThreadID;
	bool				bIsInitialized;

	void CreateAndResumeThread(SIZE_T dwStackSize, const char* apName, bool abSuspended);
	void SetThreadPriority(int aiPriority);
	void Exit();

	static void __fastcall CreateAndResumeThreadEx(BSThread* apThis, void*, SIZE_T dwStackSize, const char* apName, bool abSuspended) {
		apThis->CreateAndResumeThread(dwStackSize, apName, abSuspended);
	}

	static void __fastcall SetPreferredProcessorEx(BSThread* apThis, void*, UInt32 auiProcessor) {
		if (apThis->hCreatedThread)
			SetThreadIdealProcessor(apThis->hCreatedThread, auiProcessor);
	}
};

ASSERT_SIZE(BSThread, 0x30);
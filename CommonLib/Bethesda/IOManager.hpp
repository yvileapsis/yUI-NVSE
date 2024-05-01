#pragma once

#include "NiSmartPointer.hpp"
#include "BSTaskManager.hpp"
#include "LockFreePriorityQueue.hpp"
#include "IOTask.hpp"

class IOManager : public BSTaskManager {
public:
	UInt32 unk05C;
	UInt32 uiCurrentThreadID;
	LockFreePriorityQueue<NiPointer<IOTask>>* PostProcessQueue;
	IO_TASK_PRIORITY eLoadingQueuedPriority;
	UInt32(__thiscall* pGetExternalCountCallback)();
	void(__thiscall* pUpdateQueueCallback)();
	bool(__thiscall* pUpdateSecondaryQueueCallback)();
	void(__thiscall* pLoadQueuedPriorityCallback)(float);
	void(__thiscall* pPauseCallback)();
	void(__thiscall* pResumeCallback)();
	UInt32 unk084;
	UInt32 unk088;
	UInt32 unk08C;
	UInt32 unk090;
	UInt32 unk094;
	UInt32 unk098;
	UInt32 unk09C;

	static IOManager* GetSingleton();
	static bool IsUpdating();
	static void SetIsUpdating(bool abUpdating);
	UInt32 GetPostProcessQueueCount();
	bool RemoveTask(IOTask* apTask);
	void CancelTask(IOTask* apTask, void* apTaskOwner = nullptr);
	static bool __fastcall UpdateQueue(IOManager* apThis);

	bool IsInQueue(TESObjectREFR *refr);
	void QueueForDeletion(TESObjectREFR* refr);
	void DumpQueuedTasks();
};
ASSERT_SIZE(IOManager, 0xA0);
#pragma once

#include "BSTaskletGroup.hpp"
#include "BSTasklet.hpp"

class BSTaskletManager {
public:
	virtual ~BSTaskletManager();
	virtual void SetTaskletThreadPriority(int a2, int a3);
	virtual void ShutdownAllTasks();
	virtual bool IsTaskletThread(DWORD aiThreadID);
	virtual bool CreateTaskGroupData(BSTaskletGroupData*& appGroupData);
	virtual bool OpenTaskGroupData(BSTaskletGroupData* apGroupData);
	virtual bool AttachTaskData(BSTaskletGroupData* apGroupData, void* apUnk, bool abUnk);
	virtual bool CloseTaskGroupData(BSTaskletGroupData* apGroupData);
	virtual void DestroyTaskGroupData(BSTaskletGroupData* apGroupData);
	virtual void InstanceAvailable();

	bool	bAvailable;
	bool	bSignaled;
	LONG	uiCount;
	HANDLE	hSemaphore;
	LONG	uiMaxCount;


	bool CreateTaskGroup(BSTaskletGroup* apGroup);

	bool AttachTask(BSTaskletGroup* apGroup, BSTasklet* apTask, bool abLastTask);
};

ASSERT_SIZE(BSTaskletManager, 0x14);
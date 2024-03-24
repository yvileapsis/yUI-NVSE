#pragma once

#include "BSParallelTaskThread.hpp"
#include "BSParallelTask.hpp"

typedef void(__cdecl* BSParallelTaskThreadCallback)(BSParallelTask* apTask);

class BSParallelTaskThreadManager {
public:
	BSParallelTaskThread**				ppThreads;
	BSParallelTaskThreadCallback		pCallback;
	BSParallelTask*						pTask;
	bool								bActive;

	static BSParallelTaskThreadManager* GetSingleton();

	void AddIteratorTask(BSParallelTask* apTask);
	void AddNodeIteratorTask(BSParallelTask* apTask);
	void AddTask(BSParallelTaskThreadCallback apCallback, BSParallelTask* apTask);

	template <typename FUNC>
	void AddTask(FUNC afCallback, BSParallelTask* apTask) {
		AddTask((BSParallelTaskThreadCallback)afCallback, apTask);
	}

	void Start() const;
	void RunTask();
	void Release() const;
};
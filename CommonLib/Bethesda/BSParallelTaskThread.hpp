#pragma once

#include "BSLinearTaskThread.hpp"

class BSParallelTaskThreadManager;

class BSParallelTaskThread : public BSLinearTaskThread {
public:
	BSParallelTaskThreadManager* pManager;
	void* unk50;
	void* unk54;
	void* unk58;
};

ASSERT_SIZE(BSParallelTaskThread, 0x5C);
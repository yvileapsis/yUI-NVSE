#pragma once

#include "BSTaskThread.hpp"

class BSTaskManager;

class BSTaskManagerThread : public BSTaskThread {
public:
	BSTaskManager* pOwner;
};

ASSERT_SIZE(BSTaskManagerThread, 0x34);
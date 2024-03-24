#pragma once

#include "QueuedFileEntry.hpp"

class RadioConvTask : public QueuedFileEntry {
public:
	void* pDialogueItemList;
};

ASSERT_SIZE(RadioConvTask, 0x38);
#pragma once

#include "QueuedFile.hpp"

class QueuedReplacementKFList : public QueuedFile {
public:
	DWORD dword28;
	DWORD dword2C;
	DWORD dword30;
	DWORD dword34;
};

ASSERT_SIZE(QueuedReplacementKFList, 0x38)
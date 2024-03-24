#pragma once

#include "QueuedFileLoad.hpp"

NiSmartPointer(QueuedFaceGenFile);

struct QueuedFaceGenFile : QueuedFileLoad {
public:
	QueuedFaceGenFile();
	~QueuedFaceGenFile();

	NiRefObjectPtr nirefobjectptr38;
	NiRefObjectPtr nirefobjectptr3C;
};

ASSERT_SIZE(QueuedFaceGenFile, 0x40);
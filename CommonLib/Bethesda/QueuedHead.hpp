#pragma once

#include "QueuedFile.hpp"
#include "BSFaceGenNiNode.hpp"

class TESNPC;

NiSmartPointer(QueuedHead);

class QueuedHead : public QueuedFile {
public:
	QueuedHead();
	~QueuedHead();

	TESNPC* pNPC;				// 028
	BSFaceGenNiNodePtr spFaceNodes[2];	// 02C OBSE presumably male and female
	UInt32			unk034;				// 034
};

ASSERT_SIZE(QueuedHead, 0x38)
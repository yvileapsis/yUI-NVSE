#pragma once

#include "QueuedFileEntry.hpp"
#include "KFModel.hpp"

class QueuedKF : public QueuedFileEntry {
public:
	QueuedKF();
	~QueuedKF();

	KFModel*	pkKFModel;	// 030
	UInt8		unk034;		// 034
	UInt8		pad035[3];	// 035
};

ASSERT_SIZE(QueuedKF, 0x38)
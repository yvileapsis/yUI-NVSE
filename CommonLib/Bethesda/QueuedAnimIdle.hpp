#pragma once

#include "QueuedKF.hpp"
#include "NiObject.hpp"

class ModelLoader;

NiSmartPointer(QueuedAnimIdle);

class QueuedAnimIdle : public QueuedKF {
public:
	QueuedAnimIdle();
	~QueuedAnimIdle();

	ModelLoader* modelLoader;	// 038	Init"d by arg2
	NiPointer<NiObject>* unk03C;		// 03C	Init"d by arg1
};

ASSERT_SIZE(QueuedAnimIdle, 0x40)
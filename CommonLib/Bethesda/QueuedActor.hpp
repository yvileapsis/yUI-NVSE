#pragma once
#include "QueuedReference.hpp"

// 0x40
class QueuedActor : public QueuedReference {
public:
	QueuedActor();
	~QueuedActor();
};
static_assert(sizeof(QueuedActor) == 0x40);
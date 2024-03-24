#pragma once

#include "QueuedActor.hpp"
#include "QueuedHead.hpp"
#include "QueuedHelmet.hpp"

class QueuedCharacter : public QueuedActor {
public:
	QueuedCharacter();
	~QueuedCharacter();

	NiPointer<QueuedHead> spQueuedHead;
	NiPointer<QueuedHelmet>* spQueuedHelmet;
};
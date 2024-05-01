#pragma once
#include "QueuedActor.hpp"
#include "QueuedHead.hpp"
#include "QueuedHelmet.hpp"

// 0x48
class QueuedCharacter : public QueuedActor
{
public:
	QueuedCharacter();
	~QueuedCharacter();

	NiPointer<QueuedHead> spQueuedHead;
	NiPointer<QueuedHelmet>* spQueuedHelmet;
};
static_assert(sizeof(QueuedCharacter) == 0x48);
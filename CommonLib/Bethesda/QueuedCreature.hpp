#pragma once
#include "QueuedActor.hpp"

// 40
class QueuedCreature : public QueuedActor {
public:
	QueuedCreature();
	~QueuedCreature();
};
static_assert(sizeof(QueuedCreature) == 0x40);
#pragma once
#include "QueuedCharacter.hpp"

// 0x48
class QueuedPlayer : public QueuedCharacter 
{
public:
	QueuedPlayer();
	~QueuedPlayer();
};
static_assert(sizeof(QueuedPlayer) == 0x48);
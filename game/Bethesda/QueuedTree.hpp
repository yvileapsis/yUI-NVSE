#pragma once

#include "QueuedReference.hpp"

class QueuedTree : public QueuedReference {
public:
	QueuedTree();
	~QueuedTree();
};

ASSERT_SIZE(QueuedTree, 0x40);
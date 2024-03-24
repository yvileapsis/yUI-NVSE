#pragma once

#include "QueuedFileEntry.hpp"
#include "BSGameSound.hpp"

class AudioLoadTask : public QueuedFileEntry {
public:
	BSGameSound*	pSound;
	UInt32			unk34;
};

ASSERT_SIZE(AudioLoadTask, 0x38);
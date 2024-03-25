#pragma once
#include "QueuedFileEntry.hpp"
#include "BSGameSound.hpp"

// 0x38
class AudioLoadTask : public QueuedFileEntry {
public:
	BSGameSound*	pSound;
	UInt32			unk34;
};
static_assert(sizeof(AudioLoadTask) == 0x38);
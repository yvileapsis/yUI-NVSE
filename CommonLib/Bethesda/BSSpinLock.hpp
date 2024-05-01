#pragma once

struct BSSpinLock {
	UInt32 uiOwningThread;
	UInt32 uiLockCount;
	UInt32 unk08[6];

	void Lock();
	void Unlock();
};

ASSERT_SIZE(BSSpinLock, 0x20);
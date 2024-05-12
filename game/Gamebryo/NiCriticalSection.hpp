#pragma once

class NiCriticalSection {
public:
	CRITICAL_SECTION	m_kCriticalSection;
	UInt32				m_ulThreadOwner;
	UInt32				m_uiLockCount;

	void Lock() {
		EnterCriticalSection(&m_kCriticalSection); 
	}

	void Unlock() {
		LeaveCriticalSection(&m_kCriticalSection);
	}
};

ASSERT_SIZE(NiCriticalSection, 0x20);
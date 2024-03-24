#pragma once

class hkMultiThreadCheck {
public:
	UInt32 m_threadId;
	UInt16 m_stackTraceId;
};

ASSERT_SIZE(hkMultiThreadCheck, 0x8);
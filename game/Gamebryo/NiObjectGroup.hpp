#pragma once

#include "NiSmartPointer.hpp"

NiSmartPointer(NiObjectGroup);

class NiObjectGroup {
public:
	UInt32	m_uiSize;
	void*	m_pvBuffer;
	void*	m_pvFree;
	UInt32	m_uiRefCount;
};

ASSERT_SIZE(NiObjectGroup, 0x10);
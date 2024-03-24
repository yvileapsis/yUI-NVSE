#pragma once

#include "NiObjectNET.hpp"
#include "NiPoint3.hpp"
#include "NiTPointerMap.hpp"

class NiCloningProcess : public NiMemObject {
public:
	NiTPointerMap<NiObject*, NiObject*>*	m_pkCloneMap;
	NiTPointerMap<NiObject*, bool>*			m_pkProcessMap;
	NiObjectNET::CopyType					m_eCopyType;
	char									m_cAppendChar;
	NiPoint3								Scale;

	static NiCloningProcess* Create(NiCloningProcess* apThis, UInt32 uiHashSize = 257);
	void Destroy();
};

ASSERT_SIZE(NiCloningProcess, 0x1C);
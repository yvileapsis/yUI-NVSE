#pragma once

#include "NiMemObject.hpp"
#include "BSSpinLock.hpp"

template <class T_Data>
class NiTAbstractPoolAllocator {
public:
	class AllocNode : public NiMemObject {
	public:
		AllocNode*  m_pkNext;
		void*	   m_pkData;
		T_Data	  m_element;
	};

	static void LockCriticalSection() {
		((BSSpinLock*)0x11C5F80)->Lock();
	}
	static void UnlockCriticalSection() {
		((BSSpinLock*)0x11C5F80)->Unlock();
	}

	static AllocNode* GetBlockHeader() {
		return *(AllocNode**)0x11C5F54;
	}

	static void SetBlockHeader(AllocNode* pkBlockHeader) {
		*(AllocNode**)0x11C5F54 = pkBlockHeader;
	}

	static AllocNode* GetFreeMem() {
		return *(AllocNode**)0x11C5F58;
	}

	static void SetFreeMem(AllocNode* pkFreeMem) {
		*(AllocNode**)0x11C5F58 = pkFreeMem;
	}
};
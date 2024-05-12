#pragma once

#include "NiMemObject.hpp"
#include "NiTArray.hpp"
#include "NiCriticalSection.hpp"

class NiFixedString;

class NiGlobalStringTable : public NiMemObject {
public:
	typedef char* GlobalStringHandle;

	NiTPrimitiveArray<GlobalStringHandle>	m_kHashArray[512];
	void*									unk2000[32];
	NiCriticalSection						m_kCriticalSection;
	void*									unk20A0[24];

	static GlobalStringHandle AddString(const char* pcString);
	static void IncRefCount(GlobalStringHandle& arHandle);
	static void DecRefCount(GlobalStringHandle& arHandle);
	static UInt32 GetLength(const GlobalStringHandle& arHandle);

	static char* GetRealBufferStart(const GlobalStringHandle& arHandle);
};

ASSERT_SIZE(NiGlobalStringTable, 0x2100);
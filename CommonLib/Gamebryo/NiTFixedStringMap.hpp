#pragma once

#include "NiTMap.hpp"
#include "NiFixedString.hpp"

template <class T_Data>
class NiTFixedStringMapBase : public NiMemObject {
public:
	NiTFixedStringMapBase(UInt32 uiHashSize = 0x100);
	~NiTFixedStringMapBase();

	virtual void Destroy(bool bFreeMemory);
	virtual void Unk2();
	virtual void Unk3();

	UInt32 m_uiHashSize;
	NiTMapItem<NiFixedString, T_Data>** m_ppkHashTable;
	UInt32 m_uiCount;
};

template <class T_Data>
class NiTFixedStringMap : public NiTFixedStringMapBase<T_Data> {
public:
};

static_assert(sizeof(NiTFixedStringMapBase<UInt32>) == 0x10);
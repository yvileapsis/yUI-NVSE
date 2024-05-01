#pragma once

#include "NiTriBasedGeomData.hpp"

NiSmartPointer(NiTriStripsData);

class NiTriStripsData : public NiTriBasedGeomData {
public:
	NiTriStripsData();
	virtual ~NiTriStripsData();

	UInt16	m_usStrips;
	UInt16* m_pusStripLengths;
	UInt16* m_pusStripLists;

	CREATE_OBJECT(NiTriStripsData, 0xA75EC0);
	NIRTTI_ADDRESS(0x11F4A70);

	__forceinline UInt16 GetStripCount() const {
		return m_usStrips;
	}

	__forceinline UInt16* GetStripLengths() const {
		return m_pusStripLengths;
	}

	__forceinline UInt16* GetStripLists() const {
		return m_pusStripLists;
	}
};

ASSERT_SIZE(NiTriStripsData, 0x50);
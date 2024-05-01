#pragma once

template <class T_Data>
class hkSmallArray {
public:
	T_Data* m_data;
	UInt16	m_size;
	UInt16	m_capacityAndFlags;
};

ASSERT_SIZE(hkSmallArray<void>, 0x8);
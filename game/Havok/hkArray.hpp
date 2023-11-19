#pragma once

template <typename T_Data>
class hkArray {
public:
	T_Data* m_data;
	UInt32	m_size;
	UInt32	m_capacityAndFlags;
};
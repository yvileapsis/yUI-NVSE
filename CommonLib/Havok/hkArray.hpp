#pragma once

template <typename T_Data>
class hkArray {
public:
	T_Data* m_data;
	UInt32	m_size;
	UInt32	m_capacityAndFlags;

	UInt32 GetSize() const {
		return m_size;
	}

	T_Data* GetAt(UInt32 index) const {
		return &m_data[index];
	}
};

ASSERT_SIZE(hkArray<void>, 0xC);
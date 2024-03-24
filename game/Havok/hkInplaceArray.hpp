#pragma once

#include "hkArray.hpp"

template <typename T_Data, unsigned T_Count>
class hkInplaceArray : public hkArray<T_Data> {
public:
	T_Data m_storage[T_Count];
};
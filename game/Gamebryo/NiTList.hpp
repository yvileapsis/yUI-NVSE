#pragma once

#include "NiTPointerListBase.hpp"
#include "NiTDefaultAllocator.hpp"

template <class T_Data>
class NiTList : public NiTPointerListBase<NiTDefaultAllocator<T_Data>, T_Data> {
};
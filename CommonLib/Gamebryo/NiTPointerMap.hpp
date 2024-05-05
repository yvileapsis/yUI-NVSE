#pragma once

#include "NiTMapBase.hpp"
#include "NiTPointerAllocator.hpp"

template <class T_Key, class T_Data>
class NiTPointerMap : public NiTMapBase<NiTPointerAllocator<size_t>, T_Key, T_Data> {
public:
	NiTPointerMap(UInt32 uiHashSize = 37) : NiTMapBase<NiTPointerAllocator<size_t>, T_Key, T_Data>(uiHashSize) {}
	~NiTPointerMap() { NiTMapBase<NiTPointerAllocator<size_t>, T_Key, T_Data>::RemoveAll(); };

	virtual NiTMapItem<T_Key, T_Data>* NewItem();
	virtual void DeleteItem(NiTMapItem<T_Key, T_Data>* pkItem);
};

template<class T_Key, class T_Data>
inline NiTMapItem<T_Key, T_Data>* NiTPointerMap<T_Key, T_Data>::NewItem() {
	return (NiTMapItem<T_Key, T_Data>*)NiTMapBase<NiTPointerAllocator<size_t>, T_Key, T_Data>::m_kAllocator.Allocate();
}

template<class T_Key, class T_Data>
inline void NiTPointerMap<T_Key, T_Data>::DeleteItem(NiTMapItem<T_Key, T_Data>* pkItem) {
	pkItem->m_val = 0;
	NiTMapBase<NiTPointerAllocator<size_t>, T_Key, T_Data>::m_kAllocator.Deallocate(pkItem);
}

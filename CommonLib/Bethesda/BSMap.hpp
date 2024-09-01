#pragma once

#include "BSMapBase.hpp"

template<class T_Key, class T_Data>
class BSMap : public BSMapBase<T_Key, T_Data> {
public:
	BSMap(UInt32 uiHashSize = 37) : BSMapBase<T_Key, T_Data>(uiHashSize) {};
	~BSMap() { BSMapBase<T_Key, T_Data>::RemoveAll(); };

	virtual BSMapItem<T_Key, T_Data>* NewItem();
	virtual void DeleteItem(BSMapItem<T_Key, T_Data>* pkItem);
};

template<class T_Key, class T_Data>
inline BSMapItem<T_Key, T_Data>* BSMap<T_Key, T_Data>::NewItem() {
	return new BSMapItem<T_Key, T_Data>;
}

template<class T_Key, class T_Data>
inline void BSMap<T_Key, T_Data>::DeleteItem(BSMapItem<T_Key, T_Data>* pkItem) {
	pkItem->m_val = 0;
	delete pkItem;
}

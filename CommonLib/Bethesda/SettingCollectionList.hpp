#pragma once
#include "SettingCollection.hpp"
#include "BSSimpleList.hpp"

template<class T>
class SettingCollectionList : public SettingCollection<T> {
public:
	SettingCollectionList();
	~SettingCollectionList() override;
	BSSimpleList<T*>	kSettingList;
};
static_assert(sizeof(SettingCollectionList<void*>) == 0x114);
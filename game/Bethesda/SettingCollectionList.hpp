#pragma once

#include "SettingCollection.hpp"
#include "BSSimpleList.hpp"

template<class T>
class SettingCollectionList : public SettingCollection<T> {
public:
	BSSimpleList<T*>	SettingsA;
};

ASSERT_SIZE(SettingCollectionList<UInt32>, 0x114);
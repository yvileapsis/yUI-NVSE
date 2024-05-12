#pragma once

#include "SettingCollection.hpp"
#include "BSTCaseInsensitiveStringMap.hpp"

template<class T_Data>
class SettingCollectionMap : public SettingCollection<T_Data> {
public:
	BSTCaseInsensitiveStringMap<T_Data*> Settings;
};

ASSERT_SIZE(SettingCollection<UInt32>, 0x10C);
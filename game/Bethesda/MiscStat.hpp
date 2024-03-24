#pragma once

#include "Setting.hpp"

class MiscStat {
public:
	MiscStat();
	~MiscStat();

	Setting*	pSetting;
	SInt32		iValue;
};

ASSERT_SIZE(MiscStat, 0x8);
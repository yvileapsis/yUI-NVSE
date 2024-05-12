#pragma once

#include "TESBoundObject.hpp"
#include "TESLeveledList.hpp"

class TESLevItem : public TESBoundObject {
public:
	TESLeveledList list;
};

ASSERT_SIZE(TESLevItem, 0x4C)
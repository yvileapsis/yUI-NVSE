#pragma once

#include "BSExtraData.hpp"
#include "TESObjectREFR.hpp"

class ExtraDroppedItemList : public BSExtraData {
public:
	EXTRADATATYPE(DROPPEDITEMLIST);

	BSSimpleList<TESObjectREFR*> kDroppedItems;
};

ASSERT_SIZE(ExtraDroppedItemList, 0x14);
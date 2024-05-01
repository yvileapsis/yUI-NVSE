#pragma once
#include "BSExtraData.hpp"

// 14
class ExtraLinkedRefChildren : public BSExtraData
{
public:
	BSSimpleList<TESObjectREFR*> kLinkedRefChildren;
};
static_assert(sizeof(ExtraLinkedRefChildren) == 0x14);
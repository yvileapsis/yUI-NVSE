#pragma once
#include "BSExtraData.hpp"

class TESObjectREFR;

class ExtraAshPileRef : public BSExtraData
{
public:
	ExtraAshPileRef();
	~ExtraAshPileRef();

	TESObjectREFR* sourceRef;		// 0C
};
static_assert(sizeof(ExtraAshPileRef) == 0x10);
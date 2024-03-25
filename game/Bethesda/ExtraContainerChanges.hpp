#pragma once
#include "BSExtraData.hpp"

class TESObjectREFR;
class InventoryChanges;

// 0x10
class ExtraContainerChanges : public BSExtraData
{
public:
	ExtraContainerChanges();
	virtual ~ExtraContainerChanges();

	InventoryChanges* pkData;	// 00C
};
static_assert(sizeof(ExtraContainerChanges) == 0x10);
#pragma once
#include "TESForm.hpp"

// 0x2C
class TESCaravanDeck :
	public TESForm,
	public TESFullName
{
	BSSimpleList<void*>*	pkCards;
	UInt32					uiCount;
};
static_assert(sizeof(TESCaravanDeck) == 0x2C);
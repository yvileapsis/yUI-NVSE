#pragma once
#include "BSExtraData.hpp"

// 10
class ExtraLinkedRef : public BSExtraData
{
public:
	TESObjectREFR* linkedRef;		// 0C

	static ExtraLinkedRef* __stdcall Create(TESObjectREFR* _linkedRef = NULL);
};
static_assert(sizeof(ExtraLinkedRef) == 0x10);
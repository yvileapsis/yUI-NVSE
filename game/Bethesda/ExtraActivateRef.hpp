#pragma once
#include "BSExtraData.hpp"
#include "BSString.hpp"

// 20
class ExtraActivateRef : public BSExtraData
{
public:
	struct parentRef
	{
		TESObjectREFR* ref;
		float			delay;
	};
	BSSimpleList<parentRef*>	parentRefs;
	UInt32						flags;
	BSStringT					activationPromptOverride;
};
static_assert(sizeof(ExtraActivateRef) == 0x20);
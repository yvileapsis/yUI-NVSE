#pragma once

#include "BSExtraData.hpp"
#include "BSSimpleList.hpp"
#include "BSPointer.hpp"

class TESObjectREFR;

class ExtraReflectedRefs : public BSExtraData {
public:
	EXTRADATATYPE(REFLECTEDREFS);

	BSSimpleList<BSPointer<TESObjectREFR>*> kReflectedRefs;
};

ASSERT_SIZE(ExtraReflectedRefs, 0x14);
#pragma once
#include "BSExtraData.hpp"

class TESObjectREFR;

// 0x10
class ExtraReferencePointer : public BSExtraData {
public:
	ExtraReferencePointer();
	virtual ~ExtraReferencePointer();

	TESObjectREFR* refr;		// 00C
};
static_assert(sizeof(ExtraReferencePointer) == 0x10);
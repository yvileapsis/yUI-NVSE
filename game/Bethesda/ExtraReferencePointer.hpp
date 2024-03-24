#pragma once

#include "BSExtraData.hpp"

class TESObjectREFR;

class ExtraReferencePointer : public BSExtraData {
public:
	ExtraReferencePointer();
	virtual ~ExtraReferencePointer();

	TESObjectREFR* refr;		// 00C
};
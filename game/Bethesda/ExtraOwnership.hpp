#pragma once
#include "BSExtraData.hpp"

class TESForm;

class ExtraOwnership : public BSExtraData {
public:
	ExtraOwnership();
	virtual ~ExtraOwnership();

	TESForm* owner;	// maybe this should be a union {TESFaction*; TESNPC*} but it would be more unwieldy to access and modify

	static ExtraOwnership* Create();
};
static_assert(sizeof(ExtraOwnership) == 0x10);
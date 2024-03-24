#pragma once

#include "BSExtraData.hpp"

class ExtraAction : public BSExtraData {
public:
	ExtraAction();
	virtual ~ExtraAction();

	UInt8			flags0C;	// 00C	some kind of status or flags:
	//		if not RunOnActivateBlock: bit0 and bit1 are set before TESObjectREF::Activate, bit0 is preserved, bit1 is cleared after returning.
	UInt8			fill0D[3];	// 00D
	TESForm* actionRef;	// 010

	static ExtraAction* Create();
};
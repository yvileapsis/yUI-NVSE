#pragma once

#include "NiSmartPointer.hpp"

class BSAnimGroupSequence;
class TESAnimGroup;

class KFModel {
	const char* path;					// 000
	NiPointer<BSAnimGroupSequence> controllerSequence;	// 004
	NiPointer<TESAnimGroup> animGroup;			// 008
	UInt32				unk0C;					// 00C
	UInt32				unk10;					// 010
};

ASSERT_SIZE(KFModel, 0x14)
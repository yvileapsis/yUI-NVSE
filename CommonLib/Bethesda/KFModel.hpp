#pragma once
#include "NiSmartPointer.hpp"

class BSAnimGroupSequence;
class TESAnimGroup;

// 014
class KFModel {
public:
	const char*						pcPath;					// 000
	NiPointer<BSAnimGroupSequence>	controllerSequence;		// 004
	NiPointer<TESAnimGroup>			animGroup;				// 008
	UInt32							unk0C;					// 00C
	UInt32							unk10;					// 010
};
static_assert(sizeof(KFModel) == 0x14);
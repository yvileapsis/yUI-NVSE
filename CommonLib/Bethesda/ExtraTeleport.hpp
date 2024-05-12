#pragma once
#include "BSExtraData.hpp"
#include "NiPoint3.hpp"

class TESObjectREFR;

class ExtraTeleport : public BSExtraData {
public:
	ExtraTeleport();
	~ExtraTeleport();

	struct Data
	{
		TESObjectREFR*	pkLinkedDoor;	// 00
		NiPoint3		kPosition;
		NiPoint3		kRotation;
		UInt8			unk01C;		// 1C
		UInt8			pad01D[3];	// 1D
	};

	Data* pkData;

	static ExtraTeleport* Create();
};
static_assert(sizeof(ExtraTeleport) == 0x10);
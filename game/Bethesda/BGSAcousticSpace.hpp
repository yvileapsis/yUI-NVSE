#pragma once
#include "TESBoundObject.hpp"

class TESSound;
class TESRegion;

// 0x54
class BGSAcousticSpace : public TESBoundObject
{
public:
	BGSAcousticSpace();
	~BGSAcousticSpace();

	UInt8		bIsInterior;
	UInt8		pad31[3];
	TESSound*	pkDawnSound;
	TESSound*	pkNoonSound;
	TESSound*	pkDuskSound;
	TESSound*	pkNightSound;
	TESSound*	pkWallaSound;
	TESRegion*	pkRegion;
	UInt32		uiEnvironmentType;
	UInt32		uiWallaTriggerCount;
};
static_assert(sizeof(BGSAcousticSpace) == 0x54);
#pragma once

#include "TESBoundObject.hpp"

class TESSound;
class TESRegion;

class BGSAcousticSpace : public TESBoundObject {
public:
	BGSAcousticSpace();
	~BGSAcousticSpace();

	UInt8 isInterior;
	UInt8 pad31[3];
	TESSound* pDawnSound;
	TESSound* pNoonSound;
	TESSound* pDuskSound;
	TESSound* pNightSound;
	TESSound* pWallaSound;
	TESRegion* region;
	UInt32 environmentType;
	UInt32 wallaTriggerCount;
};

ASSERT_SIZE(BGSAcousticSpace, 0x54);
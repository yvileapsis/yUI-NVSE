#pragma once
#include "TESRegionData.hpp"

class TESSound;

// 0xC
struct SoundType {
	TESSound*		pkSound;
	UInt32			uiFlags;
	UInt32			uiChance;
};
static_assert(sizeof(SoundType) == 0xC);

// 0x20
class TESRegionDataSound : public TESRegionData {
public:
	TESRegionDataSound();
	~TESRegionDataSound();

	UInt32						unk08;					// 08
	BSSimpleList<SoundType*>	kSoundTypeList;			// 0C
	UInt32						uiIncidentalMediaSet;	// 14
	BSSimpleList<UInt32>		kMediaSetEntryList;		// 18
};
static_assert(sizeof(TESRegionDataSound) == 0x20);
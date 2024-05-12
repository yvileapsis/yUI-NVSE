#pragma once

#include "TESRegionData.hpp"

class TESSound;

struct SoundType {
	TESSound* sound;
	UInt32			flags;
	UInt32			chance;
};
typedef BSSimpleList<SoundType*> SoundTypeList;

class TESRegionDataSound : public TESRegionData {
public:
	TESRegionDataSound();
	~TESRegionDataSound();

	UInt32		unk08;				// 08
	SoundTypeList	soundTypes;			// 0C
	UInt32		incidentalMediaSet;		// 14
	BSSimpleList<UInt32>	mediaSetEntries;		// 18
};

ASSERT_SIZE(TESRegionDataSound, 0x20)
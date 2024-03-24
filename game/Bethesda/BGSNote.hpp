#pragma once

#include "TESBoundObject.hpp"
#include "TESModel.hpp"
#include "TESIcon.hpp"
#include "BGSPickupPutdownSounds.hpp"

class TESDescription;

class BGSNote : public TESBoundObject {
public:
	BGSNote();
	~BGSNote();

	// bases
	TESModel					model;					// 030
	TESFullName					fullName;				// 048
	TESIcon						icon;					// 054
	BGSPickupPutdownSounds		pickupPutdownSounds;	// 060
	TESDescription* noteText;				// 06C
	UInt32						unk070[3];				// 070
	UInt8                       unk07C;					// 07C
	UInt8                       read;					// 07D
	UInt8                       unk07E;					// 07E
	UInt8                       unk07F;					// 07F
};

ASSERT_OFFSET(BGSNote, pickupPutdownSounds, 0x60);
ASSERT_OFFSET(BGSNote, noteText, 0x06C);
ASSERT_SIZE(BGSNote, 0x80);
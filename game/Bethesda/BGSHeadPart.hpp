#pragma once

#include "TESForm.hpp"
#include "TESFullName.hpp"
#include "TESModelTextureSwap.hpp"

class BGSHeadPart : public TESForm {
public:
	BGSHeadPart();
	~BGSHeadPart();

	enum
	{
		kFlag_Playable = 0x01,
	};

	TESFullName			fullName;	// 18
	TESModelTextureSwap	texSwap;	// 24

	UInt8				headFlags;	// 44
	UInt8				pad45[3];	// 45
	UInt32				unk48;		// 48
	UInt32				unk4C;		// 4C
};

ASSERT_SIZE(BGSHeadPart, 0x50);
#pragma once

#include "TESForm.hpp"
#include "TESFullName.hpp"
#include "TESTexture.hpp"

class TESEyes : public TESForm {
public:
	TESEyes();
	~TESEyes();

	enum
	{
		kFlag_Playable = 0x01,
		kFlag_NotMale = 0x02,
		kFlag_NotFemale = 0x04,
	};

	TESFullName		fullName;	// 18
	TESTexture		texture;	// 24

	UInt8			eyeFlags;	// 30
	UInt8			pad31[3];	// 31
};

ASSERT_SIZE(TESEyes, 0x34);
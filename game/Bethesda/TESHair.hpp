#pragma once

#include "TESForm.hpp"
#include "TESFullName.hpp"
#include "TESModel.hpp"
#include "TESTexture.hpp"

class TESHair : public TESForm {
public:
	TESHair();
	~TESHair();

	enum
	{
		kFlag_Playable = 0x01,
		kFlag_NotMale = 0x02,
		kFlag_NotFemale = 0x04,
		kFlag_Fixed = 0x08,
	};

	TESFullName		fullName;	// 18
	TESModel		model;		// 24
	TESTexture		texture;	// 3C

	UInt8			hairFlags;	// 48	Playable, not Male, not Female, Fixed
	UInt8			pad49[3];	// 49
};

ASSERT_SIZE(TESHair, 0x4C);
#pragma once
#include "TESForm.hpp"
#include "TESFullName.hpp"
#include "TESModel.hpp"
#include "TESTexture.hpp"

// 4C
class TESHair : public TESForm {
public:
	TESHair();
	~TESHair();

	enum EnumHairFlags : UInt8
	{
		kFlag_Playable = 0x01,
		kFlag_NotMale = 0x02,
		kFlag_NotFemale = 0x04,
		kFlag_Fixed = 0x08,
	};

	TESFullName		kFullName;	// 18
	TESModel		kModel;		// 24
	TESTexture		kTexture;	// 3C

	EHairFlags		eHairFlags;
	UInt8			pad49[3];	// 49
};
static_assert(sizeof(TESHair) == 0x4C);
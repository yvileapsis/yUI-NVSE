#pragma once
#include "TESForm.hpp"
#include "TESFullName.hpp"
#include "TESTexture.hpp"

// 34
class TESEyes :
	public TESForm,
	public TESFullName,	// 18
	public TESTexture	// 24
{
public:
	TESEyes();
	~TESEyes();

	enum EnumEyesFlags : UInt8
	{
		kFlag_Playable	= 0x01,
		kFlag_NotMale	= 0x02,
		kFlag_NotFemale	= 0x04,
	};

	EnumEyesFlags	eEyeFlags;	// 30
	UInt8			pad31[3];	// 31
};
static_assert(sizeof(TESEyes) == 0x34);
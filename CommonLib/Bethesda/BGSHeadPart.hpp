#pragma once
#include "TESForm.hpp"
#include "TESFullName.hpp"
#include "TESModelTextureSwap.hpp"

// 50
class BGSHeadPart :
	public TESForm,					// 00
	public TESFullName,				// 18
	public TESModelTextureSwap		// 24
{
public:
	BGSHeadPart();
	~BGSHeadPart();

	enum EnumFlags : UInt8
	{
		kFlag_Playable = 0x01,
	};

	EnumFlags			eFlags;		// 44
	UInt8				pad45[3];	// 45
	UInt32				unk48;		// 48
	UInt32				unk4C;		// 4C
};
static_assert(sizeof(BGSHeadPart) == 0x50);
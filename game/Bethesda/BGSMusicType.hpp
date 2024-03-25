#pragma once
#include "TESForm.hpp"
#include "TESSoundFile.hpp"

// 0x30
class BGSMusicType :
	public TESForm,
	public TESSoundFile
{
public:
	BGSMusicType();
	~BGSMusicType();

	UInt32	unk024;
	UInt32	unk028;
	UInt32	unk02C;
};
static_assert(sizeof(BGSMusicType) == 0x30);
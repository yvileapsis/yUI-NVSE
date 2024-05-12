#pragma once
#include "TESForm.hpp"

// 0xC4
class MediaSet :
	public TESForm,
	public TESFullName
{
	struct MediaSetNameAndDB
	{
		BSStringT name;
		float decibels;
		float unk10;
	};

	enum SetType
	{
		kMediaSetType_Battle = 0x0,
		kMediaSetType_Location = 0x1,
		kMediaSetType_Dungeon = 0x2,
		kMediaSetType_Incidental = 0x3,
		kMediaSetType_None = 0x4,
	};

	UInt32 unk24;
	UInt32 unk28;
	UInt32 unk2C;
	UInt8 byte30;
	UInt8 byte31;
	UInt8 byte32;
	UInt8 byte33;
	UInt8 byte34;
	UInt8 byte35;
	UInt8 byte36;
	UInt8 byte37;
	TESFullName name38;
	UInt32 type;
	MediaSetNameAndDB loopBattleDayOuter;
	MediaSetNameAndDB exploreDayMiddle;
	MediaSetNameAndDB suspenseDayInner;
	MediaSetNameAndDB nightOuter;
	MediaSetNameAndDB nightMiddle;
	MediaSetNameAndDB nightInner;
	UInt32 enableFlags;
	float waitTime;
	float loopFadeOut;
	float recoveryTime;
	float nightTimeMax;
	TESSound *sound1;
	TESSound *sound2;
};
static_assert(sizeof(MediaSet) == 0xC4);
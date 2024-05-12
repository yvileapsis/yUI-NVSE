#pragma once
#include "TESForm.hpp"

// 0xB8
class MediaLocationController :
	public TESForm,
	public TESFullName
{
	UInt32 time24;
	UInt32 unk28;
	UInt32 unk2C;
	UInt32 unk30;
	UInt8 byte34;
	UInt8 byte35;
	UInt8 isCombatMusic;
	UInt8 byte37;
	UInt8 byte38;
	UInt8 pad39;
	UInt8 pad3A;
	UInt8 pad3B;
	Float32 unk3C;
	UInt32 unk40;
	UInt32 unk44;
	UInt32 unk48;
	UInt8 byte4C;
	UInt8 pad4D;
	UInt8 pad4E;
	UInt8 pad4F;
	MediaSet *currentTrack;
	UInt32 unk54;
	UInt32 unk58;
	UInt32 unk5C;
	TESFaction *reputation;
	UInt32 unk64;
	UInt32 unk68;
	UInt32 flags;
	Float32 flt70;
	Float32 flt74;
	Float32 retriggerDelay;
	Float32 locationDelay;
	UInt32 dayStart;
	UInt32 nightStart;
	BSSimpleList<void*> neutralSets;
	BSSimpleList<void*> allySets;
	BSSimpleList<void*> friendSets;
	BSSimpleList<void*> enemySets;
	BSSimpleList<void*> locationSets;
	BSSimpleList<void*> battleSets;
};
static_assert(sizeof(MediaLocationController) == 0xB8);
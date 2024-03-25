#pragma once
#include "TESForm.hpp"

// 0x30
class BGSEncounterZone : public TESForm {
	BGSEncounterZone();
	~BGSEncounterZone();

	enum EnumFlags : UInt8
	{
		kEncounterZone_Respawns = 0x1,
		kEncounterZone_StrictMinimumLevel = 0x2,
	};

	TESForm*	pkOwner;
	UInt8		ucRank;
	UInt8		ucMinLevel;
	EnumFlags	eFlags;
	UInt8		pad01C;
	UInt32		uiDetachTime;
	UInt32		uiAttachTime;
	UInt32		uiResetTime;
	UInt16		usZoneLevel;
};
static_assert(sizeof(BGSEncounterZone) == 0x30);
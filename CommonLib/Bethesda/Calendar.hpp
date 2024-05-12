#pragma once

#include "TESGlobal.hpp"

class Calendar {
public:
	TESGlobal*	pGameYear;
	TESGlobal*	pGameMonth;
	TESGlobal*	pGameDay;
	TESGlobal*	pGameHour;
	TESGlobal*	pGameDaysPassed;
	TESGlobal*	pTimeScale;
	UInt32		iMidnightsPassed;
	bool		bGameLoaded;
	UInt32		unk20;
	UInt32		unk24;
	UInt32		unk28;
	float		fLastUpdHour;
	UInt32		initialized;
};
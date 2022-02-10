#pragma once
#include <unordered_map>
#include "GameAPI.h"

enum eGTMValues {
	eTimeIndexDefault	= 0,
	eTimeIndexConsole	= 255,
	eTimeIndexKillCam	= 256,
	eTimeIndexTurbo		= 257,
};

inline std::unordered_map<UInt16, float> map_yGTM_LocalTimeMultsMin;
inline std::unordered_map<UInt16, float> map_yGTM_LocalTimeMultsMax;
inline float globalTimeMultMin = 1.0;
inline float globalTimeMultMax = 1.0;

void SetGlobalTimeMultiplier(TimeGlobal* timeGlobal, char isImmediateChange);
void Time_ModifyMap(const float timeMult, const UInt16 mod);

template<UInt32 retn> __declspec(naked) void DialogVATSDestructorHook() {
	SetGlobalTimeMultiplier(TimeGlobal::GetSingleton(), 1);
	static const UInt32 retnAddr = retn;
	__asm {
		mov		eax, [ebp - 0x4]
		mov		esp, ebp
		jmp		retnAddr
	}
}

template<UInt32 retn> void __fastcall SetGTMHook(TimeGlobal* timeGlobal, void* dummyEDX, float timeMult, char isImmediateChange) {
	Time_ModifyMap(timeMult, retn);
	SetGlobalTimeMultiplier(timeGlobal, isImmediateChange);
}

template<UInt32 retn> void __fastcall RestoreGTMHook(TimeGlobal* timeGlobal) {
	Time_ModifyMap(1, retn);
	SetGlobalTimeMultiplier(timeGlobal, 0);
}

inline void __fastcall SetGTMOnLoadHook(TimeGlobal* timeGlobal, void* dummyEDX, float timeMult, char isImmediateChange) {
	Time_ModifyMap(-1, eTimeIndexDefault);
	SetGlobalTimeMultiplier(timeGlobal, isImmediateChange);
}

inline void __fastcall SetGTMVATSHook(TimeGlobal* timeGlobal, void* dummyEDX, float timeMult, char isImmediateChange) {
	SetGlobalTimeMultiplier(timeGlobal, isImmediateChange);
}
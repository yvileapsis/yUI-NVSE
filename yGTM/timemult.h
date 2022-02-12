#pragma once
#include <unordered_map>
#include <unordered_set>
#include <GameAPI.h>

namespace TimeMult
{
	enum eGTMValues {
		eTimeIndexDefault = 0,
		eTimeIndexConsole = 255,
		eTimeIndexKillCam = 256,
		eTimeIndexTurbo = 257,
	};

	void FillMaps();

	inline std::unordered_set<UInt16> g_specialMods;
	inline std::unordered_map<UInt16, Float64> g_localMults;
	inline Float64 g_timeMult = 1.0;

	void Set(TimeGlobal* timeGlobal = TimeGlobal::GetSingleton(), char isImmediateChange = 1);
	void ModifyMap(Float64 timeMult, UInt16 mod);

	template<UInt32 retn> __declspec(naked) void DialogVATSDestructorHook() {
		Set(TimeGlobal::GetSingleton(), 1);
		static const UInt32 retnAddr = retn;
		__asm {
			mov		eax, [ebp - 0x4]
			mov		esp, ebp
			jmp		retnAddr
		}
	}

	template<UInt32 retn> void __fastcall SetGTMHook(TimeGlobal* timeGlobal, void* dummyEDX, float timeMult, char isImmediateChange) {
		ModifyMap(timeMult, retn);
		Set(timeGlobal, isImmediateChange);
	}

	template<UInt32 retn> void __fastcall RestoreGTMHook(TimeGlobal* timeGlobal) {
		ModifyMap(1, retn);
		Set(timeGlobal, 0);
	}

	inline void __fastcall SetGTMOnLoadHook(TimeGlobal* timeGlobal, void* dummyEDX, float timeMult, char isImmediateChange) {
		ModifyMap(0, eTimeIndexDefault);
		Set(timeGlobal, isImmediateChange);
	}

	inline void __fastcall SetGTMVATSHook(TimeGlobal* timeGlobal, void* dummyEDX, float timeMult, char isImmediateChange) {
		Set(timeGlobal, isImmediateChange);
	}
}
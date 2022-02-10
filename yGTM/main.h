#pragma once
#include <regex>
#include <SafeWrite.h>
#include <PluginAPI.h>
#include <CommandTable.h>
#include <GameUI.h>
#include <GameTasks.h>
#include <GameAPI.h>
#include <map>
#include <GameData.h>

inline DebugLog						gLog;

inline NVSEInterface*				g_nvseInterface = nullptr;
inline NVSEStringVarInterface*		g_stringInterface = nullptr;
inline NVSEArrayVarInterface*		g_arrayInterface = nullptr;
inline NVSEMessagingInterface*		g_messagingInterface = nullptr;
inline NVSEScriptInterface*			g_scriptInterface = nullptr;
inline NVSECommandTableInterface*	g_commandInterface = nullptr;
inline NVSEDataInterface*			g_dataInterface = nullptr;

inline DIHookControl*				g_DIHook = nullptr;
inline PlayerCharacter*				g_player = nullptr;
inline ActorValueOwner*				g_playerAVOwner = nullptr;
inline BaseProcess*					g_playerProcess = nullptr;
DataHandler*						g_dataHandler = nullptr;

void writePatch();
bool Cmd_SetGlobalTimeMultiplierAlt_Execute(COMMAND_ARGS);
bool Cmd_GetGlobalTimeMultiplierAlt_Execute(COMMAND_ARGS);

int iDoOnce;

std::map<UInt16, float> map_yGTM_LocalTimeMultsMin;
std::map <UInt16, float> map_yGTM_LocalTimeMultsMax;
float globalTimeMultMin = 1.0;
float globalTimeMultMax = 1.0;

extern TimeGlobal* g_timeGlobal;

void(__thiscall* TimeGlobal__SetGlobalTimeMultiplier)(TimeGlobal*, float, char) = reinterpret_cast<void(__thiscall *)(TimeGlobal*, float, char)>(0xAA4DB0);

inline void Time_ModifyMap(const float timeMult, const UInt16 mod) {

	if (timeMult < -1) {}
	else if (timeMult == -1) {
		map_yGTM_LocalTimeMultsMin.clear();
		map_yGTM_LocalTimeMultsMax.clear();
		globalTimeMultMin = 1.0;
		globalTimeMultMax = 1.0;
	}
	else if (timeMult <= 0) {}
	else if (timeMult < 1.0) {
		map_yGTM_LocalTimeMultsMin[mod] = timeMult;
		auto it = map_yGTM_LocalTimeMultsMax.find(mod);
		if (it != map_yGTM_LocalTimeMultsMax.end()) {
			map_yGTM_LocalTimeMultsMax.erase(it);
			globalTimeMultMax = 1.0;
			for (auto it = map_yGTM_LocalTimeMultsMax.rbegin(); it != map_yGTM_LocalTimeMultsMax.rend(); it++) {
				if (globalTimeMultMax < it->second) { globalTimeMultMax = it->second; }
			};
		}

		globalTimeMultMin = 1.0;
		for (auto it = map_yGTM_LocalTimeMultsMin.rbegin(); it != map_yGTM_LocalTimeMultsMin.rend(); it++) {
			if (globalTimeMultMin > it->second) { globalTimeMultMin = it->second; }
		};
	}
	else if (timeMult > 1.0) {
		map_yGTM_LocalTimeMultsMax[mod] = timeMult;
		auto it = map_yGTM_LocalTimeMultsMin.find(mod);
		if (it != map_yGTM_LocalTimeMultsMin.end()) {
			map_yGTM_LocalTimeMultsMin.erase(it);
			globalTimeMultMin = 1.0;
			for (auto it = map_yGTM_LocalTimeMultsMin.rbegin(); it != map_yGTM_LocalTimeMultsMin.rend(); it++) {
				if (globalTimeMultMin > it->second) { globalTimeMultMin = it->second; }
			};
		}

		globalTimeMultMax = 1.0;
		for (auto it = map_yGTM_LocalTimeMultsMax.rbegin(); it != map_yGTM_LocalTimeMultsMax.rend(); it++) {
			if (globalTimeMultMax < it->second) { globalTimeMultMax = it->second; }
		};
	}
	else {
		globalTimeMultMin = globalTimeMultMax = 1.0;

		auto it = map_yGTM_LocalTimeMultsMin.find(mod);
		if (it != map_yGTM_LocalTimeMultsMin.end()) map_yGTM_LocalTimeMultsMin.erase(it);

		for (auto it = map_yGTM_LocalTimeMultsMin.rbegin(); it != map_yGTM_LocalTimeMultsMin.rend(); it++) {
			if (globalTimeMultMin > it->second) { globalTimeMultMin = it->second; }
		};

		it = map_yGTM_LocalTimeMultsMax.find(mod);
		if (it != map_yGTM_LocalTimeMultsMax.end()) map_yGTM_LocalTimeMultsMax.erase(it);

		for (auto it = map_yGTM_LocalTimeMultsMax.rbegin(); it != map_yGTM_LocalTimeMultsMax.rend(); it++) {
			if (globalTimeMultMax < it->second) { globalTimeMultMax = it->second; }
		};
	}
}

inline void __fastcall TimeGlobal__SetGlobalTimeMultiplier_KillCam(TimeGlobal* timeGlobal, void* dummyEDX, float timeMult, char isImmediateChange) {
	Time_ModifyMap(timeMult, 256);
	TimeGlobal__SetGlobalTimeMultiplier(timeGlobal, globalTimeMultMin * globalTimeMultMax, isImmediateChange);
}

inline void __fastcall TimeGlobal__RestoreGlobalTimeMult_KillCam(TimeGlobal* timeGlobal) {
	Time_ModifyMap(1, 256);
	TimeGlobal__SetGlobalTimeMultiplier(timeGlobal, globalTimeMultMin * globalTimeMultMax, 1);
}

inline void __fastcall TimeGlobal__SetGlobalTimeMultiplier_Turbo(TimeGlobal* timeGlobal, void* dummyEDX, float timeMult, char isImmediateChange) {
	Time_ModifyMap(timeMult, 257);
	TimeGlobal__SetGlobalTimeMultiplier(timeGlobal, globalTimeMultMin * globalTimeMultMax, isImmediateChange);
}

inline void __fastcall TimeGlobal__RestoreGlobalTimeMult_Turbo(TimeGlobal* timeGlobal) {
	Time_ModifyMap(1, 257);
	TimeGlobal__SetGlobalTimeMultiplier(timeGlobal, globalTimeMultMin * globalTimeMultMax, 1);
}

inline void __fastcall TimeGlobal__SetGlobalTimeMultiplier_VATS(TimeGlobal* timeGlobal, void* dummyEDX, float timeMult, char isImmediateChange) {
	TimeGlobal__SetGlobalTimeMultiplier(timeGlobal, globalTimeMultMin * globalTimeMultMax, isImmediateChange);
}

inline void __fastcall TimeGlobal__SetGlobalTimeMultiplier_OnLoad(TimeGlobal* timeGlobal) {
	Time_ModifyMap(-1, 0);
	TimeGlobal__SetGlobalTimeMultiplier(timeGlobal, 1, 1);
}

inline __declspec(naked) void VATSDestructorHook() {
	TimeGlobal__SetGlobalTimeMultiplier(g_timeGlobal, globalTimeMultMin * globalTimeMultMax, 1);
	static const UInt32 retnAddr = 0x720138;
	__asm {
		mov		eax, [ebp - 0x4]
		mov		esp, ebp
		jmp		retnAddr
	}
}

inline __declspec(naked) void DialogDestructorHook() {
	TimeGlobal__SetGlobalTimeMultiplier(g_timeGlobal, globalTimeMultMin * globalTimeMultMax, 1);
	static const UInt32 retnAddr = 0x761958;
	__asm {
		mov		eax, [ebp - 0x4]
		mov		esp, ebp
		jmp		retnAddr
	}
}
// jazz code
inline __declspec(naked) bool IsConsoleOpen()
{
	__asm
	{
		mov		al, byte ptr ds : [0x11DEA2E]
		test	al, al
		jz		done
		mov		eax, fs : [0x2C]
		mov		edx, ds : [0x126FD98]
		mov		eax, [eax + edx * 4]
		test	eax, eax
		jz		done
		mov		al, [eax + 0x268]
	done :
		 retn
	}
}

inline bool Cmd_SetGlobalTimeMultiplierAlt_Execute(COMMAND_ARGS)
{
	float localTimeMult;
	localTimeMult = 1.0;
	UInt16 mod = 0;
	
	*result = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &localTimeMult, &mod))	return true;

	if (mod == 0) mod = scriptObj->GetModIndex();
	
	Time_ModifyMap(localTimeMult, mod);
	TimeGlobal__SetGlobalTimeMultiplier(g_timeGlobal, globalTimeMultMin * globalTimeMultMax, 1);

	BSAudioManager::GetSingleton()->ignoreTimescale = 0;

	if (IsConsoleOpen()) {
		Console_Print("Global Time Multiplier >> '%0.2f'", globalTimeMultMin * globalTimeMultMax);
		Console_Print("Local Time Multiplier >> '%0.2f'", localTimeMult);
	}
	*result = 1;
	return true;
}

inline bool Cmd_GetGlobalTimeMultiplierAlt_Execute(COMMAND_ARGS)
{
	float localTimeMultMin = 1.0; float localTimeMultMax = 1.0;
	SInt32 mod = 0;
	*result = 1;
	if (*reinterpret_cast<UInt16*>((static_cast<UInt8*>(scriptData) + *opcodeOffsetPtr) - 2) != 0 && !ExtractArgsEx(EXTRACT_ARGS_EX, &mod)) {
		return true;
	}
	if (mod == -1) {
		*result = *reinterpret_cast<float*>(0x11AC3A0);
	}
	else if (mod == 0) {
		auto it = map_yGTM_LocalTimeMultsMin.find(scriptObj->GetModIndex());
		if (it != map_yGTM_LocalTimeMultsMin.end()) localTimeMultMin = it->second;
		it = map_yGTM_LocalTimeMultsMax.find(scriptObj->GetModIndex());
		if (it != map_yGTM_LocalTimeMultsMax.end()) localTimeMultMax = it->second;
		*result = localTimeMultMin * localTimeMultMax;
	}
	else {
		auto it = map_yGTM_LocalTimeMultsMin.find(mod);
		if (it != map_yGTM_LocalTimeMultsMin.end()) localTimeMultMin = it->second;
		it = map_yGTM_LocalTimeMultsMax.find(mod);
		if (it != map_yGTM_LocalTimeMultsMax.end()) localTimeMultMax = it->second;
		*result = localTimeMultMin * localTimeMultMax;
	}
	
	if (IsConsoleOpen()) {
		Console_Print("Global Time Multiplier >> '%0.2f'", globalTimeMultMin * globalTimeMultMax);
		Console_Print("Local Time Multiplier >> '%0.2f'", *result);
	}
	
	return true;
}

void writePatch()
{
	WriteRelCall(0x93EC38, reinterpret_cast<UInt32>(TimeGlobal__SetGlobalTimeMultiplier_Turbo));
	WriteRelCall(0x93ECCE, reinterpret_cast<UInt32>(TimeGlobal__RestoreGlobalTimeMult_Turbo));

	WriteRelCall(0x93EAA0, reinterpret_cast<UInt32>(TimeGlobal__SetGlobalTimeMultiplier_KillCam));
	WriteRelCall(0x93E77C, reinterpret_cast<UInt32>(TimeGlobal__RestoreGlobalTimeMult_KillCam));

	WriteRelCall(0x9C7EA9, reinterpret_cast<UInt32>(TimeGlobal__SetGlobalTimeMultiplier_KillCam));
	WriteRelCall(0x9C6F3A, reinterpret_cast<UInt32>(TimeGlobal__RestoreGlobalTimeMult_KillCam));

	WriteRelCall(0x850CEE, reinterpret_cast<UInt32>(TimeGlobal__SetGlobalTimeMultiplier_OnLoad));

	WriteRelCall(0x9C8A82, reinterpret_cast<UInt32>(TimeGlobal__SetGlobalTimeMultiplier_VATS));

	WriteRelJump(0x720133, reinterpret_cast<UInt32>(VATSDestructorHook));
	WriteRelJump(0x761953, reinterpret_cast<UInt32>(DialogDestructorHook));
}

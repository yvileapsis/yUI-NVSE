#pragma once
#include <main.h>
#include <GameAPI.h>
#include <ConsoleManager.h>
#include <SafeWrite.h>

#include <SimpleINILibrary.h>

#include <ranges>
#include <unordered_set>

#include "GameData.h"
#include "Sound.h"

namespace Patch::TimeMult
{
	inline int g_yTM = 1;
	inline int g_yTM_Mode = 1;
	inline int g_yTM_MinMax = 1;

	enum eGTMValues {
		eTimeIndexDefault = 0,
		eTimeIndexConsole = 255,
		eTimeIndexKillCam = 256,
		eTimeIndexTurbo = 257,
	};

	inline std::unordered_set<UInt16> g_specialMods;
	inline std::unordered_map<UInt16, Float64> g_localMults;
	inline Float64 g_timeMult = 1.0;

	CommandInfo* cmd_GGTM = nullptr;
	CommandInfo* cmd_SGTM = nullptr;

	void Set(TimeGlobal* timeGlobal = TimeGlobal::GetSingleton(), char isImmediateChange = 1)
	{
		timeGlobal->Set(g_timeMult, isImmediateChange);
	}

	void ModifyMap(const Float64 timeMult, const UInt16 mod)
	{
		g_localMults.erase(mod);
		if (timeMult != 1) g_localMults.emplace(mod, timeMult);
		if (timeMult == 0) g_localMults.clear();

		g_timeMult = 1;

		if (g_yTM_MinMax == 0)
			for (const auto val : g_localMults | std::views::values) g_timeMult *= val;
		else if (g_yTM_MinMax == 1) {
			Float64 min = 1.0, max = 1.0;
			for (const auto val : g_localMults | std::views::values) {
				if (val < min) min = val;
				if (val > max) max = val;
			}
			g_timeMult = min * max;
		}
	}

	SInt8 wah(Script* iter, CommandInfo* command)
	{
		__try
		{
			return HasScriptCommand(iter, command, nullptr);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			return -1;
		}
	}

	void FillMaps()
	{
		std::vector<Script*> vec;

		for (auto iter : g_TESDataHandler->scriptList) vec.push_back(iter);
		for (const auto& iter : std::ranges::reverse_view(vec))
		{
			if (g_specialMods.contains(iter->modIndex)) continue;
			if (wah(iter, cmd_SGTM) == 1)
			{
				g_specialMods.emplace(iter->modIndex);
				Log(FormatString("Found SGTM use in mod: %02X (%50s), form: %08X (%50s)", iter->modIndex, GetModName(
					iter), iter->refID, iter->GetName()));
			}
			else if (wah(iter, cmd_SGTM) == -1)
			{
				Log(FormatString("Found FATAL FAILURE AND DISAPPOINTMENT use in mod: %02X (%50s), form: %08X (%50s) (to be a bit more serious for a second, this script record is bugged, please look into it)", iter->modIndex, GetModName(
					iter), iter->refID, iter->GetName()));
			}
		}
		vec.clear();
	}

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

	static ParamInfo kParams_OneOptionalInt[1] =
	{
		{	"int", kParamType_Integer, 1 },
	};
	DEFINE_COMMAND_PLUGIN(GetGlobalTimeMultiplierAlt, , 0, 2, kParams_OneOptionalInt);

	static ParamInfo kParams_OneOptionalFloat_TwoOptionalInts[3] =
	{
		{	"float", kParamType_Float, 1 },
		{	"int", kParamType_Integer, 1 },
		{	"int", kParamType_Integer, 1 },
	};
	DEFINE_COMMAND_PLUGIN(SetGlobalTimeMultiplierAlt, , 0, 3, kParams_OneOptionalFloat_TwoOptionalInts);

	extern inline bool Cmd_SetGlobalTimeMultiplierAlt_Execute(COMMAND_ARGS)
	{
		Float32 timeMult = 1.0;
		UInt32 changeTime = 0;
		UInt32 mod = 0;

		*result = 0;
		if (!ExtractArgsEx(EXTRACT_ARGS_EX, &timeMult, &changeTime, &mod)) return true;

		if (mod == 0) mod = scriptObj->modIndex;

		ModifyMap(timeMult, mod);
		Set();

		BSAudioManager::GetSingleton()->ignoreTimescale = changeTime;

		if (IsConsoleOpen()) {
			PrintConsole("Global Time Multiplier >> '%0.2f'", g_timeMult);
			PrintConsole("Local Time Multiplier >> '%0.2f'", g_localMults.contains(mod) ? g_localMults[mod] : 1.0);
		}
		*result = 1;
		return true;
	}

	extern inline bool Cmd_GetGlobalTimeMultiplierAlt_Execute(COMMAND_ARGS)
	{
		SInt32 mod = 0;

		*result = 1;
		if (*reinterpret_cast<UInt16*>(static_cast<UInt8*>(scriptData) + *opcodeOffsetPtr - 2) != 0 && !ExtractArgsEx(EXTRACT_ARGS_EX, &mod)) return true;

		if (!g_specialMods.contains(scriptObj->modIndex)) mod = -1;
		if (mod == -1) {
			mod = scriptObj->modIndex;
			*result = TimeGlobal::Get();
		}
		else {
			if (mod == 0) mod = scriptObj->modIndex;
			*result = g_localMults.contains(mod) ? g_localMults[mod] : 1.0;
		}

		if (IsConsoleOpen()) {
			PrintConsole("Global Time Multiplier >> '%0.2f'", g_timeMult);
			PrintConsole("Local Time Multiplier >> '%0.2f'", g_localMults.contains(mod) ? g_localMults[mod] : 1.0);
		}

		return true;
	}

	void PluginLoad()
	{
		cmd_SGTM = GetByOpcode(0x1186);
		cmd_GGTM = GetByOpcode(0x22B0);

		if (g_yTM)
		{
			cmd_GGTM->numParams = 1;
			cmd_GGTM->params = kParams_OneOptionalInt;

			cmd_SGTM->numParams = 3;
			cmd_SGTM->params = kParams_OneOptionalFloat_TwoOptionalInts;
		}

		if (g_nvseInterface->isEditor) return;

		if (g_yTM)
		{
			cmd_GGTM->execute = Cmd_GetGlobalTimeMultiplierAlt_Execute;
			cmd_SGTM->execute = Cmd_SetGlobalTimeMultiplierAlt_Execute;
		}
	}

	void Patch(const bool bEnable)
	{
		if (bEnable) {

			WriteRelCall(0x93EC38, SetGTMHook<eTimeIndexTurbo>);
			WriteRelCall(0x93ECCE, RestoreGTMHook<eTimeIndexTurbo>);

			WriteRelCall(0x93EAA0, SetGTMHook<eTimeIndexKillCam>);
			WriteRelCall(0x93E77C, RestoreGTMHook<eTimeIndexKillCam>);

			WriteRelCall(0x9C7EA9, SetGTMHook<eTimeIndexKillCam>);
			WriteRelCall(0x9C6F3A, RestoreGTMHook<eTimeIndexKillCam>);

			WriteRelCall(0x850CEE, SetGTMOnLoadHook);

			WriteRelCall(0x9C8A82, SetGTMVATSHook);

			WriteRelJump(0x720133, DialogVATSDestructorHook<0x720138>);
			WriteRelJump(0x761953, DialogVATSDestructorHook<0x761958>);

		}
		else {

			UndoSafeWrite(0x93EC38);
			UndoSafeWrite(0x93ECCE);

			UndoSafeWrite(0x93EAA0);
			UndoSafeWrite(0x93E77C);

			UndoSafeWrite(0x9C7EA9);
			UndoSafeWrite(0x9C6F3A);

			UndoSafeWrite(0x850CEE);

			UndoSafeWrite(0x9C8A82);

			UndoSafeWrite(0x720133);
			UndoSafeWrite(0x761953);

		}
	}

	void HandleINIs()
	{
		const auto iniPath = GetCurPath() + yUI_INI;
		CSimpleIniA ini;
		ini.SetUnicode();
		if (ini.LoadFile(iniPath.c_str()) == SI_FILE) return;

		g_yTM = ini.GetOrCreate("General", "bTimeMultChanges", 1, "; enable 'Game Time Mult' section of this .ini file.");
		g_yTM_Mode = ini.GetOrCreate("Game Time Mult", "bTimeMultMode", 1, "; select which mods are handled by yGTM, with 0 disabling handling of mods altogether, 1 handling mods that use SGTM command and 2 handling all mods.");
		g_yTM_MinMax = ini.GetOrCreate("Game Time Mult", "bTimeMultMinMax", 1, "; use multiplication of minimum and maximum local values instead of a multiplication of all local values. Provides a more sane range of TimeMult values.");

		ini.SaveFile(iniPath.c_str(), false);
	}

	extern void Init()
	{
		HandleINIs();
		if (g_yTM) pluginLoad.emplace_back(PluginLoad);

		if (g_nvseInterface->isEditor) return;
		Patch(g_yTM);
		if (g_yTM) deferredInit.emplace_back(FillMaps);
	}
}

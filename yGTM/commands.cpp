#include <CommandTable.h>
#include <GameAPI.h>
#include <GameSound.h>
#include <timeMult.h>

extern inline bool Cmd_SetGlobalTimeMultiplierAlt_Execute(COMMAND_ARGS)
{
	Float32 timeMult = 1.0;
	UInt16 mod = 0;

	*result = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &timeMult, &mod)) return true;

	if (mod == 0) mod = scriptObj->modIndex;

	TimeMult::ModifyMap(timeMult, mod);
	TimeMult::Set();

	BSAudioManager::GetSingleton()->ignoreTimescale = 0;

	if (IsConsoleOpen()) {
		PrintConsole("Global Time Multiplier >> '%0.2f'", TimeMult::g_timeMult);
		PrintConsole("Local Time Multiplier >> '%0.2f'", TimeMult::g_localMults.contains(mod) ? TimeMult::g_localMults[mod] : 1.0);
	}
	*result = 1;
	return true;
}

extern inline bool Cmd_GetGlobalTimeMultiplierAlt_Execute(COMMAND_ARGS)
{
	SInt32 mod = 0;

	*result = 1;
	if (*reinterpret_cast<UInt16*>(static_cast<UInt8*>(scriptData) + *opcodeOffsetPtr - 2) != 0 && !ExtractArgsEx(EXTRACT_ARGS_EX, &mod)) return true;

	if (!TimeMult::g_specialMods.contains(scriptObj->modIndex)) mod = -1;
	if (mod == -1) {
		mod = scriptObj->modIndex;
		*result = TimeGlobal::Get();
	} else {
		if (mod == 0) mod = scriptObj->modIndex;
		*result = TimeMult::g_localMults.contains(mod) ? TimeMult::g_localMults[mod] : 1.0;
	}

	if (IsConsoleOpen()) {
		PrintConsole("Global Time Multiplier >> '%0.2f'", TimeMult::g_timeMult);
		PrintConsole("Local Time Multiplier >> '%0.2f'", TimeMult::g_localMults.contains(mod) ? TimeMult::g_localMults[mod] : 1.0);
	}

	return true;
}
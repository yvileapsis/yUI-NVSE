#include "CommandTable.h"
#include "GameAPI.h"
#include "GameSound.h"
#include "timemult.h"

extern inline bool Cmd_SetGlobalTimeMultiplierAlt_Execute(COMMAND_ARGS)
{
	float localTimeMult;
	localTimeMult = 1.0;
	UInt16 mod = 0;

	*result = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &localTimeMult, &mod))	return true;

	if (mod == 0) mod = scriptObj->GetModIndex();

	Time_ModifyMap(localTimeMult, mod);
	SetGlobalTimeMultiplier(TimeGlobal::GetSingleton(), 1);

	BSAudioManager::GetSingleton()->ignoreTimescale = 0;

	if (IsConsoleOpen()) {
		Console_Print("Global Time Multiplier >> '%0.2f'", globalTimeMultMin * globalTimeMultMax);
		Console_Print("Local Time Multiplier >> '%0.2f'", localTimeMult);
	}
	*result = 1;
	return true;
}

extern inline bool Cmd_GetGlobalTimeMultiplierAlt_Execute(COMMAND_ARGS)
{
	float localTimeMultMin = 1.0; float localTimeMultMax = 1.0;
	SInt32 mod = 0;
	*result = 1;
	if (*reinterpret_cast<UInt16*>((static_cast<UInt8*>(scriptData) + *opcodeOffsetPtr) - 2) != 0 && !ExtractArgsEx(EXTRACT_ARGS_EX, &mod)) {
		return true;
	}
	if (mod == -1) {
		*result = TimeGlobal::Get();
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
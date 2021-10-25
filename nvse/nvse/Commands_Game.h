#pragma once

#include "CommandTable.h"
#include "ParamInfos.h"

// windows sdk sucks
#undef MessageBoxEx

DEFINE_COMMAND(GetNumericGameSetting, returns the value of a numeric game setting, 0, 1, kParams_OneString);
DEFINE_COMMAND(SetNumericGameSetting, sets a numeric game setting, 0, 2, kParams_OneString_OneFloat);
DEFINE_COMMAND(GetNumericIniSetting, returns the value of an ini setting, 0, 1, kParams_OneString);
DEFINE_COMMAND(SetNumericIniSetting, sets an ini setting, 0, 2, kParams_OneString_OneFloat);
DEFINE_COMMAND(GetCrosshairRef, returns the reference under the crosshair, 0, 0, NULL);
DEFINE_COMMAND(GetGameLoaded, returns 1 after a game is loaded, 0, 0, NULL);
DEFINE_COMMAND(GetGameRestarted, returns 1 when the game is restarted, 0, 0, NULL);
DEFINE_COMMAND(IsModLoaded, returns the whether the specified mod is loaded, 0, 1, kParams_OneString);
DEFINE_COMMAND(GetModIndex, returns the index of the specified mod, 0, 1, kParams_OneString);
DEFINE_COMMAND(GetNumLoadedMods, returns the number of loaded mods, 0, 0, NULL);
DEFINE_COMMAND(GetSourceModIndex, returns the index of the mod associated with the form, 0, 1, kParams_OneOptionalObject);
DEFINE_COMMAND(GetLocalRefIndex, returns the index of the ref, 0, 1, kParams_OneOptionalObject);
DEFINE_COMMAND(BuildRef, builds a reference from a mod and ref index, 0, 2, kParams_TwoInts);
DEFINE_COMMAND(GetDebugSelection, returns the current selected object in the console, 0, 0, NULL);
DEFINE_COMMAND(MessageEx, prints a formatted message, 0, 21, kParams_FormatString);
DEFINE_COMMAND(MessageBoxEx, displays a formatted message box, 0, 21, kParams_FormatString);

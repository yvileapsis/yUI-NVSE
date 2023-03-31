#pragma once
#include <CommandTable.h>

bool Cmd_GetyCMFloat_Execute(COMMAND_ARGS);
bool Cmd_SetyCMFloat_Execute(COMMAND_ARGS);

void WriteMCMHooks();
void FixReorderMCM();
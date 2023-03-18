#pragma once
#include <string>

#include "SafeWrite.h"

bool IsShiftHeld();
bool IsControlHeld();

int GetCharsSinceSpace(const char* text, UInt32 offset);
int GetCharsTillSpace(const char* text, UInt32 offset);

std::string GetClipboardText();

std::string ScancodeToString(UInt32 scancode);
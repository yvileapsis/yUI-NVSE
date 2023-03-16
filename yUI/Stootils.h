#pragma once
#include <string>

#include "SafeWrite.h"

bool IsShiftHeld();
bool IsControlHeld();

int GetCharsSinceSpace(const char* text, UInt32 offset);
int GetCharsTillSpace(const char* text, UInt32 offset);

std::string GetClipboardText();

std::string ScancodeToString(UInt32 scancode);
UInt32 __stdcall DetourVtable(UInt32 addr, UInt32 dst);
UInt32 __stdcall DetourRelCall(UInt32 jumpSrc, UInt32 jumpTgt);

inline Tile* (__cdecl* GetMenuTile)(UInt32 id) = (Tile * (__cdecl*)(UInt32))0xA09030;

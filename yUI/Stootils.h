#pragma once
#include <string>

#include "SafeWrite.h"

bool IsShiftHeld();
bool IsControlHeld();

int GetCharsSinceSpace(char* text, UInt32 offset);
int GetCharsTillSpace(char* text, UInt32 offset);

std::string GetClipboardText();


class DirectoryIterator
{
	HANDLE				handle;
	WIN32_FIND_DATA		fndData;

public:
	DirectoryIterator(const char* path) : handle(FindFirstFile(path, &fndData)) {}
	~DirectoryIterator() { Close(); }

	bool IsFile() const { return !(fndData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY); }
	bool IsFolder() const
	{
		if (IsFile())
			return false;
		if (fndData.cFileName[0] != '.')
			return true;
		if (fndData.cFileName[1] != '.')
			return fndData.cFileName[1] != 0;
		return fndData.cFileName[2] != 0;
	}
	const char* Get() const { return fndData.cFileName; }
	void Close()
	{
		if (handle != INVALID_HANDLE_VALUE)
		{
			FindClose(handle);
			handle = INVALID_HANDLE_VALUE;
		}
	}

	explicit operator bool() const { return handle != INVALID_HANDLE_VALUE; }
	void operator++() { if (!FindNextFile(handle, &fndData)) Close(); }
};;

std::string ScancodeToString(UInt32 scancode);
UInt32 __stdcall DetourVtable(UInt32 addr, UInt32 dst);
UInt32 __stdcall DetourRelCall(UInt32 jumpSrc, UInt32 jumpTgt);

inline Tile* (__cdecl* GetMenuTile)(UInt32 id) = (Tile * (__cdecl*)(UInt32))0xA09030;

#include "Stootils.h"

#include "Utilities.h"

struct ControlName;
// TODO: REMOVE THIS
extern DIHookControl* g_DIHook;
__declspec(naked) bool IsShiftHeld()
{
	_asm
	{
		mov ecx, g_DIHook
		cmp byte ptr[ecx + 0x12A], 0    // check left shift (DirectX scancode * 7 + 4)
		jne done
		cmp byte ptr[ecx + 0x17E], 0    // check right shift (DirectX scancode * 7 + 4)

		done:
		setne al
			ret
	}
}

_declspec(naked) bool IsAltHeld()
{
	_asm
	{
		mov edx, g_DIHook
		cmp byte ptr[edx + 0x18C], 0    // check left alt (DirectX scancode * 7 + 4)
		setne al
		ret
	}
}

_declspec(naked) bool IsTabHeld()
{
	_asm
	{
		mov edx, g_DIHook
		cmp byte ptr[edx + 0x6D], 0    // check tab (DirectX scancode * 7 + 4)
		setne al
		ret
	}
}

_declspec(naked) bool IsControlHeld()
{
	_asm
	{
		mov edx, g_DIHook
		cmp	byte ptr[edx + 0xCF], 0
		jne	done
		cmp	byte ptr[edx + 0x44F], 0
		done:
		setne al
			ret
	}
}


int GetCharsSinceSpace(const char* text, UInt32 offset)
{
	const char* barPos = text + offset;
	int numChars = 0;

	while (barPos != text && !isalnum(*--barPos)) numChars++;
	while (barPos >= text && isalnum(*barPos--)) numChars++;

	return numChars;
}

int GetCharsTillSpace(const char* text, UInt32 offset)
{
	const char* barPos = text + offset;

	int numChars = 0;

	while (isalnum(*++barPos)) numChars++;
	while (*barPos && !isalnum(*barPos++)) numChars++;

	return numChars;
}

std::string GetClipboardText()
{
	// Try opening the clipboard
	if (!OpenClipboard(nullptr))
	{
		return "";
	}

	// Get handle of clipboard object for ANSI text
	HANDLE hData = GetClipboardData(CF_TEXT);
	if (hData == nullptr)
	{
		CloseClipboard();
		return "";
	}

	// Lock the handle to get the actual text pointer
	char* pszText = static_cast<char*>(GlobalLock(hData));
	if (pszText == nullptr)
	{
		CloseClipboard();
		return "";
	}

	// Save text in a string class instance
	std::string text(pszText);

	// Release the lock
	GlobalUnlock(hData);

	// Release the clipboard
	CloseClipboard();

	return text;
}

extern ControlName** g_keyNames;
extern ControlName** g_mouseButtonNames;
extern ControlName** g_joystickNames;

std::string ScancodeToString(UInt32 scancode)
{
	std::string result;

	const char* keyName = nullptr;
	if (scancode <= 220)
	{
		if (g_keyNames[scancode])
			keyName = g_keyNames[scancode]->name;
	}
	else if (255 <= scancode && scancode <= 263)
	{
		if (scancode == 255)
			scancode = 256;
		if (g_mouseButtonNames[scancode - 256])
			keyName = g_mouseButtonNames[scancode - 256]->name;
	}
	else if (scancode == 264)		//OB doesn't provide names for wheel up/down
		keyName = "WheelUp";
	else if (scancode == 265)
		keyName = "WheelDown";
	else if (scancode == 221)
		keyName = "Select";

	if (!keyName)
	{
		char buf[0x40];
		snprintf(buf, sizeof(buf), scancode ? "Unknown Key: %d" : "--", scancode);
		result = buf;
	}
	else
	{
		result = keyName;
	}

	return result;
}



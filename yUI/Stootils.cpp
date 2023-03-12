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


int GetCharsSinceSpace(char* text, UInt32 offset)
{
	char* barPos = text + offset;
	int numChars = 0;

	while (barPos != text && !isalnum(*--barPos)) numChars++;
	while (barPos >= text && isalnum(*barPos--)) numChars++;

	return numChars;
}

int GetCharsTillSpace(char* text, UInt32 offset)
{
	char* barPos = text + offset;

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

__declspec(naked) void* __fastcall MemCopy(void *dest, const void *src, UInt32 length)
{
	__asm
	{
		push	ecx
		push	ebx
		test	ecx, ecx
		jz		done
		test	edx, edx
		jz		done
		mov		ebx, [esp+0xC]
		test	ebx, ebx
		jz		done
		mov		eax, ecx
		sub		eax, edx
		js		copy10F
		jz		done
		cmp		eax, ebx
		jb		revCopy
	copy10F:
		cmp		ebx, 0x10
		jb		copy8F
		movdqu	xmm0, xmmword ptr [edx]
		movdqu	xmmword ptr [ecx], xmm0
		sub		ebx, 0x10
		jz		done
		add		ecx, 0x10
		add		edx, 0x10
		jmp		copy10F
	copy8F:
		test	bl, 8
		jz		copy4F
		movq	xmm0, qword ptr [edx]
		movq	qword ptr [ecx], xmm0
		and		bl, 7
		jz		done
		add		ecx, 8
		add		edx, 8
	copy4F:
		test	bl, 4
		jz		copy2F
		mov		eax, [edx]
		mov		[ecx], eax
		and		bl, 3
		jz		done
		add		ecx, 4
		add		edx, 4
	copy2F:
		test	bl, 2
		jz		copy1F
		mov		ax, [edx]
		mov		[ecx], ax
		and		bl, 1
		jz		done
		add		ecx, 2
		add		edx, 2
	copy1F:
		mov		al, [edx]
		mov		[ecx], al
	done:
		pop		ebx
		pop		eax
		retn	4
	revCopy:
		add		ecx, ebx
		add		edx, ebx
	copy10B:
		cmp		ebx, 0x10
		jb		copy8B
		sub		ecx, 0x10
		sub		edx, 0x10
		movdqu	xmm0, xmmword ptr [edx]
		movdqu	xmmword ptr [ecx], xmm0
		sub		ebx, 0x10
		jz		done
		jmp		copy10B
	copy8B:
		test	bl, 8
		jz		copy4B
		sub		ecx, 8
		sub		edx, 8
		movq	xmm0, qword ptr [edx]
		movq	qword ptr [ecx], xmm0
		and		bl, 7
		jz		done
	copy4B:
		test	bl, 4
		jz		copy2B
		sub		ecx, 4
		sub		edx, 4
		mov		eax, [edx]
		mov		[ecx], eax
		and		bl, 3
		jz		done
	copy2B:
		test	bl, 2
		jz		copy1B
		sub		ecx, 2
		sub		edx, 2
		mov		ax, [edx]
		mov		[ecx], ax
		and		bl, 1
		jz		done
	copy1B:
		mov		al, [edx-1]
		mov		[ecx-1], al
		jmp		done
	}
}


__declspec(naked) char* __fastcall StrLenCopy(char* dest, const char* src, UInt32 length)
{
	__asm
	{
		test	ecx, ecx
		jz		done
		test	edx, edx
		jz		nullTerm
		mov		eax, [esp + 4]
		test	eax, eax
		jz		nullTerm
		push	eax
		call	MemCopy
		mov		ecx, eax
		add		ecx, [esp + 4]
		nullTerm:
		mov[ecx], 0
			done :
			mov		eax, ecx
			retn	4
	}
}


char* GameHeapStrdup(const char* src)
{
	if (!src) return nullptr;
	auto len = strlen(src);
	char* result = (char*)GameHeapAlloc<char*>(len + 1);
	memcpy(result, src, len + 1);
	return result;
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

[[nodiscard]] __declspec(noinline) UInt32 __stdcall DetourVtable(UInt32 addr, UInt32 dst)
{
	UInt32 originalFunction = *(UInt32*)addr;
	SafeWrite32(addr, dst);
	return originalFunction;
}

[[nodiscard]] __declspec(noinline) UInt32 __stdcall DetourRelCall(UInt32 jumpSrc, UInt32 jumpTgt)
{
	UInt32 originalFunction = *(UInt32*)(jumpSrc + 1) + jumpSrc + 5;
	WriteRelCall(jumpSrc, jumpTgt);
	return originalFunction;
}


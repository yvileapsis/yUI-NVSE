#pragma once

#include <main.h>
#include <Sound.h>
#include <SafeWrite.h>

// all credit goes to WallSoGB
namespace Patch::NameThreads
{
    static void SetThreadName(HANDLE apThread, const char* apName) {
        wchar_t wideName[1024];
        MultiByteToWideChar(CP_UTF8, 0, apName, -1, wideName, 1024);

        // Append "[FNV]" to the beginning of the thread name
        wchar_t newWideName[1024];
        wcsncpy_s(newWideName, L"[FNV] ", _TRUNCATE);
        wcsncat_s(newWideName, wideName, 1024 - wcslen(newWideName) - 1);
//        Log() << ("Creating thread %s", apName);
        SetThreadDescription(apThread, (PCWSTR)newWideName);
    }

    void __cdecl SetThreadNameAlt(UInt32 dwThreadID, const char* szThreadName) {
        HANDLE handle = OpenThread(THREAD_ALL_ACCESS, false, dwThreadID);
        SetThreadName(handle, szThreadName);
    }

    void __fastcall SetThreadNameEx(HANDLE handle, void*, const char* szThreadName) {
        SetThreadName(handle, szThreadName);
    }

    void __fastcall CreateAndResumeThread_Hook(BSThread* apThis, void*, SIZE_T dwStackSize, const char* apName, bool abSuspended) 
    {
        ThisStdCall(0xAA6430, apThis, dwStackSize, apName, abSuspended);
        SetThreadName(apThis->createdThread, apName);
    }

	void Patch()
	{
        for (int callAddr : { 0x78D37D, 0x82F8CF, 0x82F8F6, 0x8C75A4, 0x8C9B78, 0xAA81B5, 0xAD9F77, 0xBA2EB8, 0xC44AF1 }) {
            WriteRelCall(callAddr, CreateAndResumeThread_Hook);
        }

        for (int callAddr : { 0x86C56F, 0xADA005, 0xC42EB1, 0xEC1701, 0xEC197A }) {
            WriteRelCall(callAddr, SetThreadNameAlt);
        }
	}

	extern void Init()
	{
        Patch();
	}
}

#pragma once

#include <main.hpp>
#include <Safewrite.hpp>
#include <BSThread.hpp>
#include <BSWin32TaskletManager.hpp>

// all credit goes to WallSoGB
namespace Patch::NameThreads
{
	// The reason all const strings are char instead of wchar_t is to simply reduce their memory footprint

	const char cPrefix[] = "[FNV]";

	static void SetThreadName(HANDLE apThread, const char* apName) {
		char cName[32];
		sprintf_s(cName, "%s %s", cPrefix, apName);
		cName[31] = 0;

		wchar_t wcWideName[32];
		MultiByteToWideChar(CP_UTF8, 0, cName, -1, wcWideName, ARRAYSIZE(wcWideName));

		SetThreadDescription(apThread, wcWideName);
	}

	void __cdecl SetThreadNameAlt(UInt32 dwThreadID, const char* szThreadName) {
		HANDLE handle = OpenThread(THREAD_ALL_ACCESS, false, dwThreadID);
		SetThreadName(handle, szThreadName);
	}

	void __fastcall CreateAndResumeThread_Hook(BSThread* apThis, void*, SIZE_T dwStackSize, const char* apName, bool abSuspended)  {
		ThisStdCall(0xAA6430, apThis, dwStackSize, apName, abSuspended);
		if (apName && apName[0])
			SetThreadName(apThis->hCreatedThread, apName);
	}

	void NameTasklets() {
		BSWin32TaskletManager* pMgr = BSWin32TaskletManager::GetSingleton();
		const char* pMgrName = "BSWin32TaskletManager";

		char cMainName[28];
		sprintf_s(cMainName, 28, "%s %s", cPrefix, pMgrName);
		cMainName[27] = 0;

		wchar_t wcMainName[28];
		MultiByteToWideChar(CP_UTF8, 0, cMainName, -1, wcMainName, ARRAYSIZE(wcMainName));

		for (UInt8 i = 0; i < 6; i++) {
			char cName[41];
			sprintf_s(cName, 41, "%s - Tasklet %d", cMainName, i);
			cName[40] = 0;

			wchar_t wcName[41];
			MultiByteToWideChar(CP_UTF8, 0, cName, -1, wcName, ARRAYSIZE(wcName));
			SetThreadDescription(pMgr->hThreadHandles[i], wcName);
		}

		SetThreadDescription(pMgr->hInitThreadHandle, wcMainName);
	}


	void Patch()
	{
		for (int callAddr : { 0x78D37D, 0x82F8CF, 0x82F8F6, 0x8C75A4, 0x8C9B78, 0xAA81B5, 0xAD9F77, 0xBA2EB8, 0xC44AF1 }) {
			WriteRelCall(callAddr, CreateAndResumeThread_Hook);
		}

		for (int callAddr : { 0x86C56F, 0xADA005, 0xC42EB1, 0xEC1701, 0xEC197A }) {
			WriteRelCall(callAddr, SetThreadNameAlt);
		}

		NameTasklets();
	}

	extern void Init()
	{
		if (g_currentGame == kFalloutNewVegas)
			Patch();
	}
}

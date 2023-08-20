#include <filesystem>
#include <shlwapi.h>
#include <DbgHelpCrate.h>

typedef BOOL	(__stdcall* PREAD_PROCESS_MEMORY_ROUTINE)		(_In_ HANDLE hProcess, _In_ DWORD lpBaseAddress, _Out_writes_bytes_(nSize) PVOID lpBuffer, _In_ DWORD nSize, _Out_ PDWORD lpNumberOfBytesRead);
typedef BOOL	(__stdcall* PREAD_PROCESS_MEMORY_ROUTINE64)		(_In_ HANDLE hProcess, _In_ DWORD64 qwBaseAddress, _Out_writes_bytes_(nSize) PVOID lpBuffer, _In_ DWORD nSize, _Out_ LPDWORD lpNumberOfBytesRead);
typedef PVOID	(__stdcall* PFUNCTION_TABLE_ACCESS_ROUTINE)		(_In_ HANDLE hProcess, _In_ DWORD AddrBase);
typedef PVOID	(__stdcall* PFUNCTION_TABLE_ACCESS_ROUTINE64)	(_In_ HANDLE hProcess, _In_ DWORD64 AddrBase);
typedef DWORD	(__stdcall* PGET_MODULE_BASE_ROUTINE)			(_In_ HANDLE hProcess, _In_ DWORD Address);
typedef DWORD64	(__stdcall* PGET_MODULE_BASE_ROUTINE64)			(_In_ HANDLE hProcess, _In_ DWORD64 Address);
typedef DWORD	(__stdcall* PTRANSLATE_ADDRESS_ROUTINE)			(_In_ HANDLE hProcess, _In_ HANDLE hThread, _Out_ LPADDRESS lpaddr);
typedef	DWORD64	(__stdcall* PTRANSLATE_ADDRESS_ROUTINE64)		(_In_ HANDLE hProcess, _In_ HANDLE hThread, _In_ LPADDRESS64 lpaddr);

using SymGetSearchPathW_pfn		= BOOL(IMAGEAPI*)(_In_ HANDLE hProcess, _Out_ PWSTR  SearchPath, _In_  DWORD  SearchPathLength);
using SymSetSearchPathW_pfn		= BOOL(IMAGEAPI*)(_In_ HANDLE hProcess, _In_opt_ PCWSTR SearchPath);
using SymRefreshModuleList_pfn	= BOOL(IMAGEAPI*)(HANDLE hProcess);
using StackWalk64_pfn			= BOOL(IMAGEAPI*)(_In_ DWORD MachineType,_In_ HANDLE hProcess,_In_ HANDLE hThread,_Inout_ LPSTACKFRAME64 StackFrame,_Inout_ PVOID ContextRecord,_In_opt_ PREAD_PROCESS_MEMORY_ROUTINE64 ReadMemoryRoutine,_In_opt_ PFUNCTION_TABLE_ACCESS_ROUTINE64 FunctionTableAccessRoutine,_In_opt_ PGET_MODULE_BASE_ROUTINE64 GetModuleBaseRoutine,_In_opt_ PTRANSLATE_ADDRESS_ROUTINE64 TranslateAddress);
using StackWalk_pfn				= BOOL(IMAGEAPI*)(_In_ DWORD MachineType, _In_ HANDLE hProcess, _In_ HANDLE hThread, _Inout_ LPSTACKFRAME StackFrame, _Inout_ PVOID ContextRecord, _In_opt_ PREAD_PROCESS_MEMORY_ROUTINE ReadMemoryRoutine, _In_opt_ PFUNCTION_TABLE_ACCESS_ROUTINE FunctionTableAccessRoutine, _In_opt_ PGET_MODULE_BASE_ROUTINE GetModuleBaseRoutine, _In_opt_ PTRANSLATE_ADDRESS_ROUTINE	 TranslateAddress);
using SymSetOptions_pfn			= DWORD(IMAGEAPI*) (_In_ DWORD SymOptions);
using SymGetModuleBase_pfn		= DWORD(IMAGEAPI*)(_In_ HANDLE  hProcess, _In_ DWORD dwAddr);
using SymGetModuleBase64_pfn	= DWORD64(IMAGEAPI*)(_In_ HANDLE  hProcess, _In_ DWORD64 qwAddr);
using SymGetLineFromAddr_pfn	= BOOL(IMAGEAPI*)(_In_ HANDLE hProcess, _In_ DWORD dwAddr, _Out_ PDWORD pdwDisplacement, _Out_ PIMAGEHLP_LINE Line);
using SymGetLineFromAddr64_pfn	= BOOL(IMAGEAPI*)(_In_ HANDLE hProcess, _In_ DWORD64 qwAddr, _Out_ PDWORD pdwDisplacement, _Out_ PIMAGEHLP_LINE64 Line64);
using SymUnloadModule_pfn		= BOOL(IMAGEAPI*)(_In_ HANDLE hProcess, _In_ DWORD BaseOfDll);
using SymUnloadModule64_pfn		= BOOL(IMAGEAPI*)(_In_ HANDLE hProcess, _In_ DWORD64 BaseOfDll);
using SymGetTypeInfo_pfn		= BOOL(IMAGEAPI*)(_In_ HANDLE hProcess, _In_ DWORD64 ModBase, _In_ ULONG TypeId, _In_  IMAGEHLP_SYMBOL_TYPE_INFO GetType, _Out_ PVOID pInfo);
using SymFromAddr_pfn			= BOOL(IMAGEAPI*)(_In_ HANDLE hProcess, _In_ DWORD64 Address, _Out_opt_ PDWORD64 Displacement, _Inout_ PSYMBOL_INFO Symbol);
using SymInitialize_pfn			= BOOL(IMAGEAPI*)(_In_ HANDLE hProcess, _In_opt_ PCSTR UserSearchPath, _In_ BOOL fInvadeProcess);
using SymCleanup_pfn			= BOOL(IMAGEAPI*)(_In_ HANDLE hProcess);
using SymLoadModule_pfn			= DWORD(IMAGEAPI*)(_In_ HANDLE hProcess, _In_opt_ HANDLE hFile, _In_opt_ PCSTR ImageName, _In_opt_ PCSTR ModuleName, _In_ DWORD BaseOfDll, _In_ DWORD SizeOfDll);
using SymLoadModule64_pfn		= DWORD64(IMAGEAPI*)(_In_ HANDLE hProcess, _In_opt_ HANDLE hFile, _In_opt_ PCSTR ImageName, _In_opt_ PCSTR ModuleName, _In_ DWORD64 BaseOfDll, _In_ DWORD SizeOfDll);

using GetProcAddress_pfn = FARPROC(WINAPI*)(HMODULE, LPCSTR);
using SetLastError_pfn = void (WINAPI*)(_In_ DWORD dwErrCode);

static SymGetSearchPathW_pfn	SymGetSearchPathW_Imp		= nullptr;
static SymSetSearchPathW_pfn	SymSetSearchPathW_Imp		= nullptr;
static SymRefreshModuleList_pfn	SymRefreshModuleList_Imp	= nullptr;
static StackWalk64_pfn			StackWalk64_Imp				= nullptr;
static StackWalk_pfn			StackWalk_Imp				= nullptr;
static SymSetOptions_pfn		SymSetOptions_Imp			= nullptr;
static SymGetModuleBase64_pfn	SymGetModuleBase64_Imp		= nullptr;
static SymGetModuleBase_pfn		SymGetModuleBase_Imp		= nullptr;
static SymGetLineFromAddr64_pfn	SymGetLineFromAddr64_Imp	= nullptr;
static SymGetLineFromAddr_pfn	SymGetLineFromAddr_Imp		= nullptr;
static SymFromAddr_pfn			SymFromAddr_Imp				= nullptr;
static SymInitialize_pfn		SymInitialize_Imp			= nullptr;
static SymCleanup_pfn			SymCleanup_Imp				= nullptr;
static SymLoadModule_pfn		SymLoadModule_Imp			= nullptr;
static SymLoadModule64_pfn		SymLoadModule64_Imp			= nullptr;
static SymUnloadModule_pfn		SymUnloadModule_Imp			= nullptr;
static SymUnloadModule64_pfn	SymUnloadModule64_Imp		= nullptr;
static SymGetTypeInfo_pfn		SymGetTypeInfo_Imp			= nullptr;

GetProcAddress_pfn GetProcAddress_Original = nullptr;
SetLastError_pfn SetLastError_Original = nullptr;


void WINAPI SK_SetLastError(DWORD dwErrCode) noexcept
{
	__try {
		if (SetLastError_Original != nullptr) SetLastError_Original(dwErrCode);
		else SetLastError(dwErrCode);
	}
	__finally {
	}
}

FARPROC WINAPI SK_GetProcAddress(const HMODULE hMod, const char* szFunc) noexcept
{
	if (hMod == nullptr) return nullptr;

	FARPROC proc = nullptr;

	SK_SetLastError(NO_ERROR);

	if (GetProcAddress_Original != nullptr)
	{
		proc = GetProcAddress_Original(hMod, szFunc);
		return (GetLastError() == NO_ERROR) ? proc : nullptr;
	}

	proc = GetProcAddress(hMod, szFunc);
	return (GetLastError() == NO_ERROR) ? proc : nullptr;
}

wchar_t* SKX_GetInstallPath(void)
{
	static wchar_t SK_InstallPath[MAX_PATH + 2] = { };
	return SK_InstallPath;
}

const wchar_t* __stdcall SK_GetInstallPath(void)
{
	return SKX_GetInstallPath();
}


bool __stdcall SK_Assert_SameDLLVersion(const wchar_t* wszTestFile0, const wchar_t* wszTestFile1)
{
	std::wstring wszVerStr0 = L"", wszVerStr1 = L"";

	using test_pair_s = std::pair<std::wstring&, const wchar_t*>;
	test_pair_s tests[] = { { wszVerStr0, wszTestFile0 },  { wszVerStr1, wszTestFile1 } };

	for (auto& [wszVerStr, wszTestFile] : tests)
	{
		UINT cbTranslatedBytes = 0, cbProductBytes = 0, cbVersionBytes = 0;

		uint8_t cbData[16384] = { };
		size_t dwSize = 16383;

		wchar_t* wszFileDescrip = nullptr; // Will point somewhere in cbData
		wchar_t* wszFileVersion = nullptr; // "

		struct LANGANDCODEPAGE {
			WORD wLanguage;
			WORD wCodePage;
		} *lpTranslate = nullptr;

		BOOL bRet = SK_GetFileVersionInfoExW(FILE_VER_GET_NEUTRAL | FILE_VER_GET_PREFETCHED, wszTestFile,
				0x00, static_cast <DWORD> (dwSize), cbData);

		SK_ReleaseAssert(L"File Has No Version Info?!" && bRet == TRUE);
		if (!bRet) { return bRet; }

		if (SK_VerQueryValueW(cbData, TEXT("\\VarFileInfo\\Translation"), static_cast_p2p <void>(&lpTranslate),
			&cbTranslatedBytes) && cbTranslatedBytes && lpTranslate)
		{
			wchar_t wszPropName[64] = { };
			_snwprintf_s(wszPropName, 63, LR"(\StringFileInfo\%04x%04x\FileDescription)",
				lpTranslate[0].wLanguage, lpTranslate[0].wCodePage);

			SK_VerQueryValueW(cbData, wszPropName, static_cast_p2p <void>(&wszFileDescrip), &cbProductBytes);

			_snwprintf_s(wszPropName, 63, LR"(\StringFileInfo\%04x%04x\FileVersion)",
				lpTranslate[0].wLanguage, lpTranslate[0].wCodePage);

			SK_VerQueryValueW(cbData, wszPropName, static_cast_p2p <void>(&wszFileVersion), &cbVersionBytes);
		}

		if (cbTranslatedBytes == 0 || (cbProductBytes == 0 && cbVersionBytes == 0))
		{
			SK_ReleaseAssert(L"Version Info Contains No Version...");

			return false;
		}

		if (cbVersionBytes)
		{
			wszVerStr = wszFileVersion;
		}
	}

	return (!wszVerStr0.empty()) && wszVerStr0._Equal(wszVerStr1);
};


DWORD WINAPI SK_timeGetTime(void) noexcept
{
	LARGE_INTEGER qpcNow;
	if (SK_QueryPerformanceCounter(&qpcNow)) return static_cast <DWORD> ((qpcNow.QuadPart / SK_QpcTicksPerMs) & 0xFFFFFFFFLL);
	
	static HMODULE hModWinMM = LoadLibraryEx(L"winmm.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	static timeGetTime_pfn winmm_timeGetTime = (timeGetTime_pfn)GetProcAddress(hModWinMM, "timeGetTime");
	
	return winmm_timeGetTime();
}

void
SK_File_MoveNoFail(const wchar_t* wszOld, const wchar_t* wszNew)
{
	WIN32_FIND_DATA OldFileData = { };
	HANDLE hOldFind = FindFirstFile(wszOld, &OldFileData);

	// Strip read-only if need be
	SK_File_SetNormalAttribs(wszNew);

	if (!MoveFileExW(wszOld, wszNew, MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED))
	{
		wchar_t wszTemp[MAX_PATH + 2] = { };
		GetTempFileNameW(SK_SYS_GetInstallPath().c_str(), L"SKI", SK_timeGetTime(), wszTemp);

		MoveFileExW(wszNew, wszTemp, MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED);
		MoveFileExW(wszOld, wszNew, MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED);
	}

	// Preserve file times
	if (hOldFind != INVALID_HANDLE_VALUE)
	{
		SK_AutoHandle hNewFile(CreateFileW(wszNew, GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr,
			OPEN_EXISTING, GetFileAttributesW(wszNew), nullptr));

		FindClose(hOldFind);
		SetFileTime(hNewFile, &OldFileData.ftCreationTime,
			&OldFileData.ftLastAccessTime, &OldFileData.ftLastWriteTime);
		SetFileAttributesW(wszNew, OldFileData.dwFileAttributes);
	}
}

uint64_t SK_DeleteTemporaryFiles(const wchar_t* wszPath, const wchar_t* wszPattern)
{
	WIN32_FIND_DATA fd = { };
	size_t files = 0UL;
	LARGE_INTEGER liSize = { 0ULL };

	wchar_t		  wszFindPattern[MAX_PATH + 2] = { };
	SK_PathCombineW(wszFindPattern, wszPath, wszPattern);

	HANDLE hFind = FindFirstFileW(wszFindPattern, &fd);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		if (config.system.log_level >= 0)
		{
			dll_log->LogEx(true, L"[Clean Mgr.] Cleaning temporary files in '%s'...	",
				SK_ConcealUserDir(std::wstring(wszPath).data()));
		}

		wchar_t wszFullPath[MAX_PATH + 2] = { };

		do
		{
			if (fd.dwFileAttributes != INVALID_FILE_ATTRIBUTES)
			{
				*wszFullPath = L'\0';

				SK_PathCombineW(wszFullPath, wszPath, fd.cFileName);

				if (DeleteFileW(wszFullPath))
				{
					++files;

					liSize.QuadPart += LARGE_INTEGER{ fd.nFileSizeLow,
													   (LONG)fd.nFileSizeHigh }.QuadPart;
				}
			}
		} while (FindNextFileW(hFind, &fd) != 0);

//		if (config.system.log_level >= 0)
//			dll_log->LogEx(false, L"%zu files deleted\n", files);

		FindClose(hFind);
	}

	return (uint64_t)liSize.QuadPart;
}

bool __stdcall SK_CreateDirectories(const wchar_t* wszPath)
{
	return SK_CreateDirectoriesEx(wszPath, false);// true);
}

std::wstring SK_EvalEnvironmentVars(const wchar_t* wszEvaluateMe)
{
	if (!wszEvaluateMe) return std::wstring();
	wchar_t wszEvaluated[MAX_PATH * 4 + 2] = { };
	ExpandEnvironmentStringsW(wszEvaluateMe, wszEvaluated, MAX_PATH * 4);
	return wszEvaluated;
}

/*
	class skModuleRegistry
	{
	public:

		static constexpr HMODULE INVALID_MODULE = nullptr;

		inline bool isValid(HMODULE const hModTest) const noexcept
		{
			return (hModTest > (HMODULE)nullptr //&&
				);// hModTest != INVALID_MODULE );
		}

		HMODULE getLibrary(const wchar_t* wszLibrary, bool add_ref = true, bool force_load = true)
		{
			const bool bEmpty = _known_module_names.empty();

			if (bEmpty && (!force_load)) return INVALID_MODULE;

			HMODULE hMod = bEmpty ? INVALID_MODULE : _FindLibraryByName(wszLibrary);

			bool ref_added = false;

			if (hMod == INVALID_MODULE && force_load)
			{
				hMod = LoadLibraryLL(wszLibrary);

				if (hMod != INVALID_MODULE) ref_added = true;
			}

			if (hMod != INVALID_MODULE)
			{
				if (add_ref == false && ref_added == true)
				{
					std::scoped_lock <SK_Thread_HybridSpinlock> auto_lock(*SK_DLL_LoaderLockGuard());
					_loaded_libraries[hMod].Release();
				}

				return hMod;
			}

			return INVALID_MODULE;
		}

		HMODULE
			getLoadedLibrary(const wchar_t* wszLibrary, bool add_ref = false)
		{
			if (_known_module_names.empty())
				return INVALID_MODULE;

			HMODULE hMod = _FindLibraryByName(wszLibrary);

			if (hMod == INVALID_MODULE)
			{
				hMod = SK_GetModuleHandle(wszLibrary);

				if (hMod != INVALID_MODULE) _RegisterLibrary(hMod, wszLibrary);
			}

			if (hMod != INVALID_MODULE)
			{
				if (add_ref == true)
				{
					std::scoped_lock <SK_Thread_HybridSpinlock> auto_lock(*SK_DLL_LoaderLockGuard());
					_loaded_libraries[hMod].AddRef();
				}

				return hMod;
			}

			return INVALID_MODULE;
		}

	private:
		// Don't forget to free anything you find!
		HMODULE _FindLibraryByName(const wchar_t* wszLibrary)
		{
			std::scoped_lock <SK_Thread_HybridSpinlock> auto_lock(*SK_DLL_LoaderLockGuard());

			if (_known_module_names.empty()) return INVALID_MODULE;

			const auto it = _known_module_names.find(wszLibrary);

			if (it != _known_module_names.cend())
			{
				_loaded_libraries[it->second].AddRef();
				return (it->second);
			}

			return INVALID_MODULE;
		}

		bool _RegisterLibrary(HMODULE hMod, const wchar_t* wszLibrary)
		{
			MODULEINFO mod_info = { };

			//BOOL bHasValidInfo =
			GetModuleInformation(GetCurrentProcess(), hMod, &mod_info, sizeof(MODULEINFO));

			std::scoped_lock <SK_Thread_HybridSpinlock> auto_lock(
				*SK_DLL_LoaderLockGuard()
			);

			//assert (bHasValidInfo); // WTF?

			if (_loaded_libraries.find(hMod) !=
				_loaded_libraries.cend())
			{
				return false;
			}

			return
				_loaded_libraries.emplace(hMod,
					skWin32Module(hMod, mod_info, wszLibrary)
				).second;
		}

		// Returns INVALID_MODULE if no more references exist
		HMODULE _ReleaseLibrary(skWin32Module& library)
		{
			std::scoped_lock <SK_Thread_HybridSpinlock> auto_lock(
				*SK_DLL_LoaderLockGuard()
			);

			assert(library != INVALID_MODULE);

			if (library.Release() == 0)
			{
				if (SK_FreeLibrary(library))
					return INVALID_MODULE;
			}

			return library;
		}

		// With benefits
		friend class
			::skWin32Module;

	public:

		// Special References that are valid for SK's entire lifetime
		static skWin32Module& HostApp(HMODULE hModToSet = skWin32Module::Uninitialized);
		static skWin32Module& Self(HMODULE hModToSet = skWin32Module::Uninitialized);


	public:
		//
		//	( LL  =  Low-Level  or  Lethal-Liability! )
		//
		// This will bypass SK's DLL monitoring facilities and is capable
		//   of loading a library without SK noticing and analyzing hook
		//	 dependencies.
		//
		//  ** More importantly, it can subvert third-party software that is
		//	   tracing LoadLibrary (...) calls.
		//
		//   > This is extremely unsafe and should only be used during SK init <
		//
		HMODULE
			LoadLibraryLL(const wchar_t* wszLibrary)
		{
			if (wszLibrary == nullptr)
				return INVALID_MODULE;

			HMODULE hMod =
				_FindLibraryByName(wszLibrary);

			if (hMod != INVALID_MODULE)
				return hMod;

			hMod =
				SK_LoadLibraryW(wszLibrary);

			// We're not fooling anyone, third-party software can see this!
			///assert (LoadLibraryW_Original != nullptr);

			if (hMod != INVALID_MODULE)
				_RegisterLibrary(hMod, wszLibrary);

			return hMod;
		}

		HMODULE
			LoadSystemLibrary(const wchar_t* wszLibrary)
		{
			if (wszLibrary == nullptr)
				return INVALID_MODULE;

			wchar_t wszFullPath[MAX_PATH * 4] = { };

#ifdef _M_IX86
			GetSystemWow64DirectoryW(wszFullPath, MAX_PATH * 3);
#else
			GetSystemDirectoryW(wszFullPath, MAX_PATH * 3);
#endif

			PathAppendW(wszFullPath, wszLibrary);

			HMODULE hMod =
				_FindLibraryByName(wszFullPath);

			if (hMod != INVALID_MODULE)
				return hMod;

			hMod =
				LoadLibraryW(wszFullPath);

			// We're not fooling anyone, third-party software can see this!
			///assert (LoadLibraryW_Original != nullptr);

			if (hMod != INVALID_MODULE)
				_RegisterLibrary(hMod, wszFullPath);

			return hMod;
		}

		HMODULE
			LoadLibrary(const wchar_t* wszLibrary)
		{
			if (wszLibrary == nullptr)
				return INVALID_MODULE;

			HMODULE hMod =
				_FindLibraryByName(wszLibrary);

			if (hMod != INVALID_MODULE)
				return hMod;

			hMod =
				LoadLibraryW(wszLibrary);

			if (hMod != INVALID_MODULE)
				_RegisterLibrary(hMod, wszLibrary);

			return hMod;
		}


		// Win32 API doesn't normally allow for freeing libraries by name, it's
		//   not a good practice... but it's convenient.
		HMODULE
			FreeLibrary(const wchar_t* wszLibrary)
		{
			const auto it =
				_known_module_names.find(std::wstring(wszLibrary));

			if (it != _known_module_names.cend())
			{
				return
					_ReleaseLibrary(_loaded_libraries[it->second]);
			}

			assert(!"FreeLibrary (...) on unknown module!"); // This is why freeing libraries by name is a bad idea!

			return skWin32Module::Uninitialized;
		}

		HMODULE
			FreeLibrary(skWin32Module&& module)
		{
			assert((HMODULE)module != 0);
			return _ReleaseLibrary(module);
		}


		// This is severely lacking in thread-safety, but Windows holds a loader
		//   lock on most module manipulation functions anyway.
		//
		//   ** It WOULD be nice to support concurrent module name lookups with
		//	  out locking, but this is already more complicated than needed.
	protected:
		std::unordered_map  <LPVOID, HMODULE>		_known_module_bases;
		std::unordered_map  <std::wstring, HMODULE>		_known_module_names;

		std::unordered_map  <HMODULE, skWin32Module>  _loaded_libraries;
	};
	*/


	HMODULE SK_Debug_LoadHelper(void)
	{
		static HMODULE hModDbgHelp = nullptr;
		static volatile LONG __init = 0;

		// Isolate and load the system DLL as a different module since
		//   dbghelp.dll is not threadsafe and other software may be using
		//	 the system DLL.
		if (!InterlockedCompareExchangeAcquire(&__init, 1, 0))
		{
			static std::filesystem::path path_to_driver =
				std::filesystem::path(SK_GetInstallPath()) / LR"(Drivers\Dbghelp\)";

			wchar_t wszSystemDbgHelp[MAX_PATH + 2] = { };
			wchar_t wszIsolatedDbgHelp[MAX_PATH + 2] = { };

			GetSystemDirectoryW(wszSystemDbgHelp, MAX_PATH);
			PathAppendW(wszSystemDbgHelp, L"dbghelp.dll");

			lstrcatW(wszIsolatedDbgHelp, path_to_driver.c_str());
			PathAppendW(wszIsolatedDbgHelp, BITNESS == 64 ? L"dbghelp_sk64.dll" : L"dbghelp_sk32.dll");

			if (FALSE == PathFileExistsW(wszIsolatedDbgHelp) ||
				FALSE == SK_Assert_SameDLLVersion(wszIsolatedDbgHelp, wszSystemDbgHelp))
			{
				if (PathFileExistsW(wszIsolatedDbgHelp))
				{

/*				SK_LOGi0(
					L"Performing in-place upgrade for %ws",
					SK_StripUserNameFromPathW(
						(std::wstring(wszIsolatedDbgHelp) + L'\0').data()
					)
				);*/

					wchar_t wszTemp[MAX_PATH + 2] = { };

					GetTempFileNameW(path_to_driver.c_str(), L"SKI", SK_timeGetTime(), wszTemp);
					SK_File_MoveNoFail(wszIsolatedDbgHelp, wszTemp);
				}

				SK_DeleteTemporaryFiles(path_to_driver.c_str());

				SK_CreateDirectories(wszIsolatedDbgHelp);
				CopyFileW(wszSystemDbgHelp, wszIsolatedDbgHelp, TRUE);
			}

			hModDbgHelp = LoadLibraryW(wszIsolatedDbgHelp);

			InterlockedIncrementRelease(&__init);
		}

		else SK_Thread_SpinUntilAtomicMin(&__init, 2);

		return hModDbgHelp;
	}


	void SK_DbgHlp_Init(void)
	{
		static volatile LONG __init = 0;

		if (!InterlockedCompareExchangeAcquire(&__init, 1, 0))
		{
			const auto helperDll = SK_Debug_LoadHelper();

			SymGetSearchPathW_Imp = (SymGetSearchPathW_pfn) SK_GetProcAddress(helperDll, "SymGetSearchPathW");
			SymSetSearchPathW_Imp =	(SymSetSearchPathW_pfn) SK_GetProcAddress(SK_Debug_LoadHelper(), "SymSetSearchPathW");
			SymRefreshModuleList_Imp = (SymRefreshModuleList_pfn) SK_GetProcAddress(SK_Debug_LoadHelper(), "SymRefreshModuleList");
			StackWalk64_Imp = (StackWalk64_pfn) SK_GetProcAddress(SK_Debug_LoadHelper(), "StackWalk64");
			StackWalk_Imp = (StackWalk_pfn) SK_GetProcAddress(SK_Debug_LoadHelper(), "StackWalk");
			SymSetOptions_Imp = (SymSetOptions_pfn) SK_GetProcAddress(SK_Debug_LoadHelper(), "SymSetOptions");
			SymGetModuleBase64_Imp = (SymGetModuleBase64_pfn) SK_GetProcAddress(SK_Debug_LoadHelper(), "SymGetModuleBase64");
			SymGetModuleBase_Imp = (SymGetModuleBase_pfn) SK_GetProcAddress(SK_Debug_LoadHelper(), "SymGetModuleBase");
			SymGetLineFromAddr64_Imp = (SymGetLineFromAddr64_pfn) SK_GetProcAddress(SK_Debug_LoadHelper(), "SymGetLineFromAddr64");
			SymGetLineFromAddr_Imp = (SymGetLineFromAddr_pfn) SK_GetProcAddress(SK_Debug_LoadHelper(), "SymGetLineFromAddr");
			SymInitialize_Imp = (SymInitialize_pfn) SK_GetProcAddress(SK_Debug_LoadHelper(), "SymInitialize");
			SymUnloadModule_Imp = (SymUnloadModule_pfn) SK_GetProcAddress(SK_Debug_LoadHelper(), "SymUnloadModule");
			SymUnloadModule64_Imp = (SymUnloadModule64_pfn) SK_GetProcAddress(SK_Debug_LoadHelper(), "SymUnloadModule64");
			SymFromAddr_Imp = (SymFromAddr_pfn) SK_GetProcAddress(SK_Debug_LoadHelper(), "SymFromAddr");
			SymCleanup_Imp = (SymCleanup_pfn) SK_GetProcAddress(SK_Debug_LoadHelper(), "SymCleanup");
			SymLoadModule_Imp = (SymLoadModule_pfn) SK_GetProcAddress(SK_Debug_LoadHelper(), "SymLoadModule");
			SymLoadModule64_Imp = (SymLoadModule64_pfn) SK_GetProcAddress(SK_Debug_LoadHelper(), "SymLoadModule64");
			SymGetTypeInfo_Imp = (SymGetTypeInfo_pfn) SK_GetProcAddress(SK_Debug_LoadHelper(), "SymGetTypeInfo");

			InterlockedIncrementRelease(&__init);
		}

		else
			SK_Thread_SpinUntilAtomicMin(&__init, 2);
	}
};
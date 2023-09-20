#include <windows.h>
#include <dbghelp.h>
#include <mutex>
#include <functional>
#include <stdexcept>
#include <iostream>
#include <filesystem>
#include <map>
#include <Logging.h>

// Class representing the DbgHelp library
class DbgHelpWrapper {
public:
	static DbgHelpWrapper& GetSingleton() 
	{
		static DbgHelpWrapper instance; // Guaranteed to be destroyed and instantiated on first use
		return instance;
	}

	// Unload the DbgHelp library
	~DbgHelpWrapper() {
		if (dbghelp_dll) {
			FreeLibrary(dbghelp_dll);
		}

		std::filesystem::remove(dll_path);
	}

	// Wrapper template for the API functions
	template<typename RetType, typename... Args>
	static RetType CallFunction(const char* func_name, Args&... args) {

		auto& dbghelp = DbgHelpWrapper::GetSingleton();

		RetType (WINAPI* func_ptr)(Args...) = dbghelp.GetCachedProcAddress<RetType(WINAPI*)(Args...)>(func_name);

		if (func_ptr) {
			return func_ptr(std::forward<Args>(args)...);
		} else {
			Log() << ("Cannot find the specified function in DbgHelp.dll");
		}
		return 0;
	}

	// Wrapper template for the API functions
	template<typename RetType, typename... Args>
	static RetType CallFunctionSafe(const char* func_name, Args&... args) {

		auto& dbghelp = DbgHelpWrapper::GetSingleton();
		std::lock_guard<std::mutex> lock(dbghelp.mutex);

		RetType (WINAPI* func_ptr)(Args...) = dbghelp.GetCachedProcAddress<RetType(WINAPI*)(Args...)>(func_name);

		if (func_ptr) {
			return func_ptr(std::forward<Args>(args)...);
		} else {
			Log() << ("Cannot find the specified function in DbgHelp.dll");
		}
		return 0;
	}

private:
	HMODULE dbghelp_dll;
	std::filesystem::path dll_path;
	std::mutex mutex;
	std::map<std::string, FARPROC> func_cache;

	// Load the DbgHelp library
	DbgHelpWrapper() {
		// Find the DbgHelp.dll path
		WCHAR dbghelp_path[MAX_PATH];
		GetSystemDirectoryW(dbghelp_path, MAX_PATH);
		wcscat_s(dbghelp_path, L"\\dbghelp.dll");

		// Create a copy of the original DbgHelp.dll
		WCHAR temp_dbghelp_path[MAX_PATH];
		GetTempPathW(MAX_PATH, temp_dbghelp_path);
		wcscat_s(temp_dbghelp_path, L"nvse_dbghelp.dll");

		if (!CopyFileW(dbghelp_path, temp_dbghelp_path, FALSE)) {
			Log() << ("Cannot create a copy of DbgHelp.dll");
			return;
		}

		dbghelp_dll = LoadLibraryW(temp_dbghelp_path);
		if (!dbghelp_dll) {
			std::filesystem::remove(temp_dbghelp_path);
			Log() << ("Cannot load DbgHelp.dll");
			return;
		}

		dll_path = temp_dbghelp_path;
	}

	DbgHelpWrapper(const DbgHelpWrapper&) = delete;
	DbgHelpWrapper& operator=(const DbgHelpWrapper&) = delete;


	template<typename T>
	T GetCachedProcAddress(const char* function_name) {
		auto it = func_cache.find(function_name);
		if (it != func_cache.end()) {
			return reinterpret_cast<T>(it->second);
		}

		FARPROC func_ptr = GetProcAddress(dbghelp_dll, function_name);
		if (func_ptr) {
			func_cache.emplace(function_name, func_ptr);
			return reinterpret_cast<T>(func_ptr);
		}

		Log() << ("Cannot find the specified function in DbgHelp.dll");

		return nullptr;
	}
};

BOOL __stdcall  Safe_SymInitializeW(HANDLE hProcess, PCWSTR UserSearchPath, BOOL fInvadeProcess) {
	return DbgHelpWrapper::CallFunctionSafe<BOOL, HANDLE, PCWSTR, BOOL>("SymInitializeW", hProcess, UserSearchPath, fInvadeProcess);
}

BOOL __stdcall  Safe_SymGetSearchPathW(HANDLE hProcess, PCWSTR SearchPath, DWORD SearchPathLength) {
	return DbgHelpWrapper::CallFunctionSafe<BOOL, HANDLE, PCWSTR, DWORD>("SymGetSearchPathW", hProcess, SearchPath, SearchPathLength);
}

BOOL __stdcall  Safe_SymSetSearchPathW(HANDLE hProcess, PCWSTR SearchPath) {
	return DbgHelpWrapper::CallFunctionSafe<BOOL, HANDLE, PCWSTR>("SymSetSearchPathW", hProcess, SearchPath);
}

BOOL __stdcall  Safe_SymRefreshModuleList(HANDLE hProcess) {
	return DbgHelpWrapper::CallFunctionSafe<BOOL, HANDLE>("SymRefreshModuleList", hProcess);
}

BOOL __stdcall  Safe_StackWalk64(_In_ DWORD MachineType,_In_ HANDLE hProcess,_In_ HANDLE hThread,_Inout_ LPSTACKFRAME64 StackFrame,_Inout_ PVOID ContextRecord,_In_opt_ PREAD_PROCESS_MEMORY_ROUTINE64 ReadMemoryRoutine,_In_opt_ PFUNCTION_TABLE_ACCESS_ROUTINE64 FunctionTableAccessRoutine,_In_opt_ PGET_MODULE_BASE_ROUTINE64 GetModuleBaseRoutine,_In_opt_ PTRANSLATE_ADDRESS_ROUTINE64 TranslateAddress) {
	return DbgHelpWrapper::CallFunctionSafe<BOOL, DWORD, HANDLE, HANDLE, LPSTACKFRAME64, PVOID, PREAD_PROCESS_MEMORY_ROUTINE64, PFUNCTION_TABLE_ACCESS_ROUTINE64, PGET_MODULE_BASE_ROUTINE64, PTRANSLATE_ADDRESS_ROUTINE64>("StackWalk64", MachineType, hProcess, hThread, StackFrame, ContextRecord, ReadMemoryRoutine, FunctionTableAccessRoutine, GetModuleBaseRoutine, TranslateAddress);
}

BOOL __stdcall  Safe_StackWalk(_In_ DWORD MachineType, _In_ HANDLE hProcess, _In_ HANDLE hThread, _Inout_ LPSTACKFRAME StackFrame, _Inout_ PVOID ContextRecord, _In_opt_ PREAD_PROCESS_MEMORY_ROUTINE ReadMemoryRoutine, _In_opt_ PFUNCTION_TABLE_ACCESS_ROUTINE FunctionTableAccessRoutine, _In_opt_ PGET_MODULE_BASE_ROUTINE GetModuleBaseRoutine, _In_opt_ PTRANSLATE_ADDRESS_ROUTINE	 TranslateAddress) {
	return DbgHelpWrapper::CallFunctionSafe<BOOL, DWORD, HANDLE, HANDLE, LPSTACKFRAME, PVOID, PREAD_PROCESS_MEMORY_ROUTINE, PFUNCTION_TABLE_ACCESS_ROUTINE, PGET_MODULE_BASE_ROUTINE, PTRANSLATE_ADDRESS_ROUTINE>("StackWalk", MachineType, hProcess, hThread, StackFrame, ContextRecord, ReadMemoryRoutine, FunctionTableAccessRoutine, GetModuleBaseRoutine, TranslateAddress);
}

DWORD __stdcall  Safe_SymSetOptions(DWORD SymOptions) {
	return DbgHelpWrapper::CallFunctionSafe<DWORD, DWORD>("SymSetOptions", SymOptions);
}

DWORD __stdcall  Safe_SymGetModuleBase(_In_ HANDLE hProcess, _In_ DWORD dwAddr) {
	return DbgHelpWrapper::CallFunction<DWORD, HANDLE, DWORD>("SymGetModuleBase", hProcess, dwAddr);
}

DWORD64 __stdcall  Safe_SymGetModuleBase64(_In_ HANDLE hProcess, _In_ DWORD dwAddr) {
	return DbgHelpWrapper::CallFunctionSafe<DWORD64, HANDLE, DWORD>("SymGetModuleBase64", hProcess, dwAddr);
}

BOOL __stdcall  Safe_SymGetLineFromAddr(_In_ HANDLE hProcess, _In_ DWORD dwAddr, _Out_ PDWORD pdwDisplacement, _Out_ PIMAGEHLP_LINE Line) {
	return DbgHelpWrapper::CallFunctionSafe<BOOL, HANDLE, DWORD, PDWORD, PIMAGEHLP_LINE>("SymGetLineFromAddr", hProcess, dwAddr, pdwDisplacement, Line);
}

BOOL __stdcall  Safe_SymGetLineFromAddr64(_In_ HANDLE hProcess, _In_ DWORD64 qwAddr, _Out_ PDWORD pdwDisplacement, _Out_ PIMAGEHLP_LINE64 Line64) {
	return DbgHelpWrapper::CallFunctionSafe<BOOL, HANDLE, DWORD64, PDWORD, PIMAGEHLP_LINE64>("SymGetLineFromAddr64", hProcess, qwAddr, pdwDisplacement, Line64);
}

BOOL __stdcall  Safe_SymUnloadModule(_In_ HANDLE hProcess, _In_ DWORD BaseOfDll) {
	return DbgHelpWrapper::CallFunctionSafe<BOOL, HANDLE, DWORD>("SymUnloadModule", hProcess, BaseOfDll);
}

BOOL __stdcall  Safe_SymUnloadModule64(_In_ HANDLE hProcess, _In_ DWORD64 BaseOfDll) {
	return DbgHelpWrapper::CallFunctionSafe<BOOL, HANDLE, DWORD64>("SymUnloadModule64", hProcess, BaseOfDll);
}

BOOL __stdcall  Safe_SymGetTypeInfo(_In_ HANDLE hProcess, _In_ DWORD64 ModBase, _In_ ULONG TypeId, _In_  IMAGEHLP_SYMBOL_TYPE_INFO GetType, _Out_ PVOID pInfo) {
	return DbgHelpWrapper::CallFunctionSafe<BOOL, HANDLE, DWORD64, ULONG, IMAGEHLP_SYMBOL_TYPE_INFO, PVOID>("SymGetTypeInfo", hProcess, ModBase, TypeId, GetType, pInfo);
}

BOOL __stdcall  Safe_SymFromAddr(_In_ HANDLE hProcess, _In_ DWORD64 Address, _Out_opt_ PDWORD64 Displacement, _Inout_ PSYMBOL_INFO Symbol) {
	return DbgHelpWrapper::CallFunctionSafe<BOOL, HANDLE, DWORD64, PDWORD64, PSYMBOL_INFO>("SymFromAddr", hProcess, Address, Displacement, Symbol);
}

BOOL __stdcall  Safe_SymInitialize(_In_ HANDLE hProcess, _In_opt_ PCSTR UserSearchPath, _In_ BOOL fInvadeProcess) {
	return DbgHelpWrapper::CallFunctionSafe<BOOL, HANDLE, PCSTR, BOOL>("SymInitialize", hProcess, UserSearchPath, fInvadeProcess);
}

BOOL __stdcall  Safe_SymCleanup(_In_ HANDLE hProcess){
	return DbgHelpWrapper::CallFunctionSafe<BOOL, HANDLE>("SymCleanup", hProcess);
}

DWORD __stdcall  Safe_SymLoadModule(_In_ HANDLE hProcess, _In_opt_ HANDLE hFile, _In_opt_ PCSTR ImageName, _In_opt_ PCSTR ModuleName, _In_ DWORD BaseOfDll, _In_ DWORD SizeOfDll) {
	return DbgHelpWrapper::CallFunctionSafe<DWORD, HANDLE, HANDLE, PCSTR, PCSTR, DWORD, DWORD>("SymLoadModule", hProcess, hFile, ImageName, ModuleName, BaseOfDll, SizeOfDll);
}

DWORD64 __stdcall  Safe_SymLoadModule64(_In_ HANDLE hProcess, _In_opt_ HANDLE hFile, _In_opt_ PCSTR ImageName, _In_opt_ PCSTR ModuleName, _In_ DWORD64 BaseOfDll, _In_ DWORD SizeOfDll) {
	return DbgHelpWrapper::CallFunctionSafe<DWORD64, HANDLE, HANDLE, PCSTR, PCSTR, DWORD64, DWORD>("SymLoadModule", hProcess, hFile, ImageName, ModuleName, BaseOfDll, SizeOfDll);
}

BOOL __stdcall  Safe_EnumerateLoadedModules(_In_ HANDLE hProcess, _In_ PENUMLOADED_MODULES_CALLBACK EnumLoadedModulesCallback, _In_opt_ PVOID UserContext) {
	return DbgHelpWrapper::CallFunctionSafe<BOOL, HANDLE, PENUMLOADED_MODULES_CALLBACK, PVOID>("EnumerateLoadedModules", hProcess, EnumLoadedModulesCallback, UserContext);
}

BOOL __stdcall Safe_SymGetModuleInfo(_In_ HANDLE hProcess, _In_ DWORD dwAddr, _Out_ PIMAGEHLP_MODULE ModuleInfo) {
	return DbgHelpWrapper::CallFunctionSafe<BOOL, HANDLE, DWORD, PIMAGEHLP_MODULE>("SymGetModuleInfo", hProcess, dwAddr, ModuleInfo);
}

BOOL __stdcall Safe_SymGetSymFromAddr(_In_ HANDLE hProcess, _In_ DWORD dwAddr, _Out_opt_ PDWORD pdwDisplacement, _Inout_ PIMAGEHLP_SYMBOL Symbol) {
	return DbgHelpWrapper::CallFunctionSafe<BOOL, HANDLE, DWORD, PDWORD, PIMAGEHLP_SYMBOL>("SymGetSymFromAddr", hProcess, dwAddr, pdwDisplacement, Symbol);
}

PVOID __stdcall Safe_SymFunctionTableAccess(_In_ HANDLE hProcess, _In_ DWORD AddrBase) {
	return DbgHelpWrapper::CallFunction<PVOID, HANDLE, DWORD>("SymFunctionTableAccess", hProcess, AddrBase);
}
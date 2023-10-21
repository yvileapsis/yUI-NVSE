#pragma once
#include <DbgHelp.h>

BOOL __stdcall Safe_SymInitializeW(HANDLE hProcess, PCWSTR UserSearchPath, BOOL fInvadeProcess);
BOOL __stdcall  Safe_SymGetSearchPathW(HANDLE hProcess, PCWSTR SearchPath, DWORD SearchPathLength);
BOOL __stdcall  Safe_SymSetSearchPathW(HANDLE hProcess, PCWSTR SearchPath);
BOOL __stdcall  Safe_SymRefreshModuleList(HANDLE hProcess);
BOOL __stdcall  Safe_StackWalk(_In_ DWORD MachineType, _In_ HANDLE hProcess, _In_ HANDLE hThread, _Inout_ LPSTACKFRAME StackFrame, _Inout_ PVOID ContextRecord, _In_opt_ PREAD_PROCESS_MEMORY_ROUTINE ReadMemoryRoutine, _In_opt_ PFUNCTION_TABLE_ACCESS_ROUTINE FunctionTableAccessRoutine, _In_opt_ PGET_MODULE_BASE_ROUTINE GetModuleBaseRoutine, _In_opt_ PTRANSLATE_ADDRESS_ROUTINE	 TranslateAddress);
BOOL __stdcall  Safe_StackWalk64(_In_ DWORD MachineType, _In_ HANDLE hProcess, _In_ HANDLE hThread, _Inout_ LPSTACKFRAME64 StackFrame, _Inout_ PVOID ContextRecord, _In_opt_ PREAD_PROCESS_MEMORY_ROUTINE64 ReadMemoryRoutine, _In_opt_ PFUNCTION_TABLE_ACCESS_ROUTINE64 FunctionTableAccessRoutine, _In_opt_ PGET_MODULE_BASE_ROUTINE64 GetModuleBaseRoutine, _In_opt_ PTRANSLATE_ADDRESS_ROUTINE64 TranslateAddress);
DWORD __stdcall  Safe_SymSetOptions(DWORD SymOptions);
DWORD __stdcall  Safe_SymGetModuleBase(_In_ HANDLE hProcess, _In_ DWORD dwAddr);
DWORD64 __stdcall  Safe_SymGetModuleBase64(_In_ HANDLE hProcess, _In_ DWORD dwAddr);
BOOL __stdcall  Safe_SymGetLineFromAddr(_In_ HANDLE hProcess, _In_ DWORD dwAddr, _Out_ PDWORD pdwDisplacement, _Out_ PIMAGEHLP_LINE Line);
BOOL __stdcall  Safe_SymGetLineFromAddr64(_In_ HANDLE hProcess, _In_ DWORD64 qwAddr, _Out_ PDWORD pdwDisplacement, _Out_ PIMAGEHLP_LINE64 Line64);
BOOL __stdcall  Safe_SymUnloadModule(_In_ HANDLE hProcess, _In_ DWORD BaseOfDll);
BOOL __stdcall  Safe_SymUnloadModule64(_In_ HANDLE hProcess, _In_ DWORD64 BaseOfDll);
BOOL __stdcall  Safe_SymGetTypeInfo(_In_ HANDLE hProcess, _In_ DWORD64 ModBase, _In_ ULONG TypeId, _In_  IMAGEHLP_SYMBOL_TYPE_INFO GetType, _Out_ PVOID pInfo);
BOOL __stdcall  Safe_SymFromAddr(_In_ HANDLE hProcess, _In_ DWORD64 Address, _Out_opt_ PDWORD64 Displacement, _Inout_ PSYMBOL_INFO Symbol);
BOOL __stdcall  Safe_SymInitialize(_In_ HANDLE hProcess, _In_opt_ PCSTR UserSearchPath, _In_ BOOL fInvadeProcess);
BOOL __stdcall  Safe_SymCleanup(_In_ HANDLE hProcess);
DWORD __stdcall  Safe_SymLoadModule(_In_ HANDLE hProcess, _In_opt_ HANDLE hFile, _In_opt_ PCSTR ImageName, _In_opt_ PCSTR ModuleName, _In_ DWORD BaseOfDll, _In_ DWORD SizeOfDll);
DWORD64 __stdcall  Safe_SymLoadModule64(_In_ HANDLE hProcess, _In_opt_ HANDLE hFile, _In_opt_ PCSTR ImageName, _In_opt_ PCSTR ModuleName, _In_ DWORD64 BaseOfDll, _In_ DWORD SizeOfDll);
BOOL __stdcall  Safe_EnumerateLoadedModules(_In_ HANDLE hProcess, _In_ PENUMLOADED_MODULES_CALLBACK EnumLoadedModulesCallback, _In_opt_ PVOID UserContext);
BOOL __stdcall Safe_SymGetModuleInfo(_In_ HANDLE hProcess, _In_ DWORD dwAddr, _Out_ PIMAGEHLP_MODULE ModuleInfo);
BOOL __stdcall Safe_SymGetSymFromAddr(_In_ HANDLE hProcess, _In_ DWORD dwAddr, _Out_opt_ PDWORD pdwDisplacement, _Inout_ PIMAGEHLP_SYMBOL Symbol);
PVOID __stdcall Safe_SymFunctionTableAccess(_In_ HANDLE hProcess, _In_ DWORD AddrBase);
DWORD __stdcall Safe_UnDecorateSymbolName(_In_ PCSTR name, _Out_writes_(maxStringLength) PSTR outputString, _In_ DWORD maxStringLength, _In_ DWORD flags);
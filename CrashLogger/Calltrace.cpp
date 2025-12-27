#include <CrashLogger.hpp>

namespace CrashLogger::Playtime
{
	std::chrono::time_point<std::chrono::system_clock> gameStart;

	extern void Init() { gameStart = std::chrono::system_clock::now(); }

	extern void __fastcall Process(EXCEPTION_POINTERS* info) {
		try {
			std::chrono::time_point<std::chrono::system_clock> gameEnd = std::chrono::system_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::seconds>(gameEnd - gameStart);

			auto hours = duration.count() / 3600;
			auto mins = (duration.count() % 3600) / 60;
			auto secs = duration.count() % 60;

			_MESSAGE("\nPlaytime: %02lld:%02lld:%02lld", hours, mins, secs);
		}
		catch (...) { 
			_MESSAGE("\nFailed to log playtime."); 
		}
	}
}

namespace CrashLogger::Exception
{
	extern void __fastcall Process(EXCEPTION_POINTERS* info) {
		try {
			_MESSAGE("\nException: %s (%08X)", GetExceptionAsString(info->ExceptionRecord->ExceptionCode), info->ExceptionRecord->ExceptionCode);
			if (GetLastError()) {
				char sanitizedBuffer[256] = {};
				_MESSAGE("Last Error: %s (%08X)", SanitizeString(GetErrorAsString(GetLastError()).c_str(), sanitizedBuffer, sizeof(sanitizedBuffer)), GetLastError());
			}
		}
		catch (...) {
			_MESSAGE("\nFailed to log exception.");
		}
	}
}

namespace CrashLogger::Thread
{
	const char* __fastcall GetThreadName(char* buffer, size_t bufferSize)
	{
		wchar_t* pThreadName = NULL;
		HRESULT hr = GetThreadDescription(GetCurrentThread(), &pThreadName);

		if (SUCCEEDED(hr) && pThreadName) {
			size_t convertedChars = 0;
			wcstombs_s(&convertedChars, buffer, bufferSize, pThreadName, _TRUNCATE);
		}
		else {
			strcpy_s(buffer, bufferSize, "Unknown");
		}

		LocalFree(pThreadName);
		return buffer;
	}

	extern void __fastcall Process(EXCEPTION_POINTERS* info) {
		try { 
			char buffer[256];
			_MESSAGE("\nThread: %s", GetThreadName(buffer, sizeof(buffer))); 
		}
		catch (...) { 
			_MESSAGE("Failed to log thread name."); 
		}
	}
}

namespace CrashLogger::Calltrace
{
	void __fastcall GetCalltraceFunction(UInt32 eip, UInt32 ebp, HANDLE process, char* buffer, SIZE_T bufferSize)
	{
		/*if (GetModuleFileName((HMODULE)frame.AddrPC.Offset, path, MAX_PATH)) {  //Do this work on non base addresses even on  Windows? Cal directly the LDR function?
		if (!SymLoadModule(process, NULL, path, NULL, 0, 0)) Log() << FormatString("Porcoddio %0X", GetLastError());
		}*/

		const auto moduleBase = PDB::GetModuleBase(eip, process);

        char begin[16];
        sprintf_s(begin, "0x%08X | ", ebp);

        char middle[MAX_PATH] = {};

        const auto moduleOffset = (moduleBase != 0x00400000) ? eip - moduleBase + 0x10000000 : eip;

		{
			char module[MAX_PATH] = {};
			char symbol[MAX_PATH] = {};
			if (!PDB::GetModule(eip, process, module, sizeof(module)) || !module[0])
				sprintf_s(middle, "%28s (0x%08X) | %-40s |", "-\\(°_o)/-", moduleOffset, "(Corrupt stack or heap?)");
			else if (!PDB::GetSymbol(eip, process, symbol, sizeof(symbol)) || !symbol[0])
				sprintf_s(middle, "%28s (0x%08X) | %-40s |", module, moduleOffset, "");
			else
				sprintf_s(middle, "%28s (0x%08X) | %-40s |", module, moduleOffset, symbol);
		}

		char end[MAX_PATH] = {};
		PDB::GetLine(eip, process, end, sizeof(end));

		sprintf_s(buffer, bufferSize, "%s%s %s", begin, middle, end);
	}

	extern void __fastcall Process(EXCEPTION_POINTERS* info) {
			HANDLE process = GetCurrentProcess();
			HANDLE thread = GetCurrentThread();

			DWORD machine = IMAGE_FILE_MACHINE_I386;
			CONTEXT context = {};
			memcpy(&context, info->ContextRecord, sizeof(CONTEXT));

			Safe_SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_DEFERRED_LOADS | SYMOPT_UNDNAME | SYMOPT_ALLOW_ABSOLUTE_SYMBOLS);

			char buffer[2048] = {};
			{
				char pathBuffer[MAX_PATH];
				GetCurrentDirectory(sizeof(pathBuffer), pathBuffer);
				sprintf_s(buffer, "%s;%s\\Data\\NVSE\\Plugins", pathBuffer, pathBuffer);
				GetEnvironmentVariable("_NT_SYMBOL_PATH", pathBuffer, sizeof(pathBuffer));
				sprintf_s(buffer, "%s;%s", buffer, pathBuffer);
				GetEnvironmentVariable("_NT_ALTERNATE_SYMBOL_PATH ", pathBuffer, sizeof(pathBuffer));
				sprintf_s(buffer, "%s;%s", buffer, pathBuffer);

				if (!Safe_SymInitialize(process, buffer, true))
					_MESSAGE("Error initializing symbol store");
			}

			STACKFRAME frame = {};
			frame.AddrPC.Offset = info->ContextRecord->Eip;
			frame.AddrPC.Mode = AddrModeFlat;
			frame.AddrFrame.Offset = info->ContextRecord->Ebp;
			frame.AddrFrame.Mode = AddrModeFlat;
			frame.AddrStack.Offset = info->ContextRecord->Esp;
			frame.AddrStack.Mode = AddrModeFlat;
			DWORD eip = 0;

			_MESSAGE("\nCalltrace:\n %*s%*s |  %*s%*s | %*s%*s |  Source", CENTERED_TEXT(10, "ebp"), CENTERED_TEXT(40, "Address"), CENTERED_TEXT(40, "Function"));

			AutoIndent indent;
		try {
			while (Safe_StackWalk(machine, process, thread, &frame, &context, NULL, Safe_SymFunctionTableAccess, Safe_SymGetModuleBase, NULL)) {
				/*
				Using  a PDB for OBSE from VS2019 is causing the frame to repeat, but apparently only if WINEDEBUG=+dbghelp isn't setted. Is this a wine issue?
				When this happen winedbg show only the first line (this happens with the first frame only probably, even if there are more frames shown when using WINEDEBUG=+dbghelp )
				*/
				if (frame.AddrPC.Offset == eip) break;
				eip = frame.AddrPC.Offset;
				GetCalltraceFunction(frame.AddrPC.Offset, frame.AddrFrame.Offset, process, buffer, sizeof(buffer));
				_MESSAGE(buffer);
			}
		}
		catch (...) {
			_MESSAGE("Failed to log callstack.");
		}
	}
}
#include <CrashLogger.hpp>

namespace CrashLogger::Playtime
{
	std::stringstream output;

	std::chrono::time_point<std::chrono::system_clock> gameStart;
	std::chrono::time_point<std::chrono::system_clock> gameEnd;

	extern void Init() { gameStart = std::chrono::system_clock::now(); }

	extern void Process(EXCEPTION_POINTERS* info)
	try
	{
		gameEnd = std::chrono::system_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::seconds>(gameEnd - gameStart);

		char textBuffer[32];
		auto hours = duration.count() / 3600;
		auto mins = (duration.count() % 3600) / 60;
		auto secs = duration.count() % 60;

		sprintf_s(textBuffer, sizeof(textBuffer), "Playtime: %02lld:%02lld:%02lld\n", hours, mins, secs);
		output << textBuffer;
	}
	catch (...) { output << "Failed to log playtime.\n"; }

	extern std::stringstream& Get() { output.flush(); return output; }

}

namespace CrashLogger::Exception
{
	std::stringstream output;

	extern void Process(EXCEPTION_POINTERS* info)
	try
	{
		char textBuffer[128];
		sprintf_s(textBuffer, "Exception: %s (%08X)\n", GetExceptionAsString(info->ExceptionRecord->ExceptionCode), info->ExceptionRecord->ExceptionCode);
		output << textBuffer;
		if (GetLastError()) {
			sprintf_s(textBuffer, "Last Error: %s (%08X)\n", SanitizeString(GetErrorAsString(GetLastError())).c_str(), GetLastError());
			output << textBuffer;
		}
	}
	catch (...) { output << "Failed to log exception.\n"; }

	extern std::stringstream& Get() { output.flush(); return output; }
}

namespace CrashLogger::Thread
{
	std::stringstream output;

	std::string GetThreadName()
	{
		std::string threadName;
		wchar_t* pThreadName = NULL;
		HRESULT hr = GetThreadDescription(GetCurrentThread(), &pThreadName);
		std::wstring wThreadName(pThreadName);
		std::transform(wThreadName.begin(), wThreadName.end(), std::back_inserter(threadName), [] (wchar_t c) { return (char)c; });
		LocalFree(pThreadName);
		return threadName;
	}

	extern void Process(EXCEPTION_POINTERS* info)
	try { output << "Thread: " << GetThreadName() << '\n'; }
	catch (...) { output << "Failed to log thread name.\n"; }

	extern std::stringstream& Get() { output.flush(); return output; }
}

namespace CrashLogger::Calltrace
{
	std::stringstream output;

	void GetCalltraceFunction(UInt32 eip, UInt32 ebp, HANDLE process, char* buffer, SIZE_T bufferSize)
	{
		/*if (GetModuleFileName((HMODULE)frame.AddrPC.Offset, path, MAX_PATH)) {  //Do this work on non base addresses even on  Windows? Cal directly the LDR function?
		if (!SymLoadModule(process, NULL, path, NULL, 0, 0)) Log() << FormatString("Porcoddio %0X", GetLastError());
		}*/

		const auto moduleBase = PDB::GetModuleBase(eip, process);

        char begin[16];
        sprintf_s(begin, "0x%08X | ", ebp);

        char middle[128] = {};

        const auto moduleOffset = (moduleBase != 0x00400000) ? eip - moduleBase + 0x10000000 : eip;

        if (const auto module = PDB::GetModule(eip, process); module.empty()) 
			sprintf_s(middle, "%28s (0x%08X) | %-40s |", "-\\(°_o)/-", moduleOffset, "(Corrupt stack or heap?)");
        else if (const auto symbol = PDB::GetSymbol(eip, process); symbol.empty())
			sprintf_s(middle, "%28s (0x%08X) | %-40s |", module.c_str(), moduleOffset, "");
        else
			sprintf_s(middle, "%28s (0x%08X) | %-40s |", module.c_str(), moduleOffset, symbol.c_str());

		char end[MAX_PATH] = {};

		if (const auto line = PDB::GetLine(eip, process); !line.empty())
		{
			sprintf_s(end, " %s", line.c_str());
		}

		sprintf_s(buffer, bufferSize, "%s%s%s", begin, middle, end);
	}

	extern void Process(EXCEPTION_POINTERS* info) 
	try {
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

			//	SymSetExtendedOption((IMAGEHLP_EXTENDED_OPTIONS)SYMOPT_EX_WINE_NATIVE_MODULES, TRUE);
			if (!Safe_SymInitialize(process, buffer, true))
				output << "Error initializing symbol store\n";

			//	SymSetExtendedOption((IMAGEHLP_EXTENDED_OPTIONS)SYMOPT_EX_WINE_NATIVE_MODULES, TRUE);
		}

		STACKFRAME frame = {};
		frame.AddrPC.Offset = info->ContextRecord->Eip;
		frame.AddrPC.Mode = AddrModeFlat;
		frame.AddrFrame.Offset = info->ContextRecord->Ebp;
		frame.AddrFrame.Mode = AddrModeFlat;
		frame.AddrStack.Offset = info->ContextRecord->Esp;
		frame.AddrStack.Mode = AddrModeFlat;
		DWORD eip = 0;

		sprintf_s(buffer, "Calltrace:\n%*s%*s |  %*s%*s | %*s%*s |  Source\n", CENTERED_TEXT(10, "ebp"), CENTERED_TEXT(40, "Address"), CENTERED_TEXT(40, "Function"));
		output << buffer;

		while (Safe_StackWalk(machine, process, thread, &frame, &context, NULL, Safe_SymFunctionTableAccess, Safe_SymGetModuleBase, NULL)) {
			/*
			Using  a PDB for OBSE from VS2019 is causing the frame to repeat, but apparently only if WINEDEBUG=+dbghelp isn't setted. Is this a wine issue?
			When this happen winedbg show only the first line (this happens with the first frame only probably, even if there are more frames shown when using WINEDEBUG=+dbghelp )
			*/
			if (frame.AddrPC.Offset == eip) break;
			eip = frame.AddrPC.Offset;
			GetCalltraceFunction(frame.AddrPC.Offset, frame.AddrFrame.Offset, process, buffer, sizeof(buffer));
			output << buffer << '\n';
		}
	}
	catch (...) {  output << "Failed to log callstack.\n"; }

	extern std::stringstream& Get() { output.flush(); return output; }
}
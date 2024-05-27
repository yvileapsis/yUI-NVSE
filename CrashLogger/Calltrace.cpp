#include <CrashLogger.hpp>
#include <exception>

// TODO: write/find a converter of exception codes to strings
namespace CrashLogger::Exception
{
	std::stringstream output;

	extern void Process(EXCEPTION_POINTERS* info)
	try
	{
		output << std::format("Exception: {} ({:08X})\n",  GetExceptionAsString(info->ExceptionRecord->ExceptionCode), info->ExceptionRecord->ExceptionCode);
		if (GetLastError()) output << std::format("Last Error: {} ({:08X})\n", SanitizeString(GetErrorAsString(GetLastError())), GetLastError());
	}
	catch (...) { output << "Failed to log exception." << '\n'; }

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
	catch (...) { output << "Failed to log thread name." << '\n'; }

	extern std::stringstream& Get() { output.flush(); return output; }
}

namespace CrashLogger::Calltrace
{
	std::stringstream output;

	std::string GetCalltraceFunction(UInt32 eip, UInt32 ebp, HANDLE process)
	{
		/*if (GetModuleFileName((HMODULE)frame.AddrPC.Offset, path, MAX_PATH)) {  //Do this work on non base addresses even on  Windows? Cal directly the LDR function?
		if (!SymLoadModule(process, NULL, path, NULL, 0, 0)) Log() << FormatString("Porcoddio %0X", GetLastError());
		}*/

		const auto moduleBase = PDB::GetModuleBase(eip, process);

		std::string begin = std::format("0x{:08X} | ", ebp);

		std::string middle;

		const auto moduleOffset = (moduleBase != 0x00400000) ? eip - moduleBase + 0x10000000 : eip;

		if (const auto module = PDB::GetModule(eip, process); module.empty()) 
			middle = std::format("{:>28s} (0x{:08X}) | {:<40s} |", "-\\(°_o)/-", moduleOffset, "(Corrupt stack or heap?)");
		else if (const auto symbol = PDB::GetSymbol(eip, process); symbol.empty())
			middle = std::format("{:>28s} (0x{:08X}) | {:<40s} |", module, moduleOffset, "");
		else
			middle = std::format("{:>28s} (0x{:08X}) | {:<40s} |", module, moduleOffset, symbol);

		std::string end;

		if (const auto line = PDB::GetLine(eip, process); !line.empty())
		{
			end = " " + line;
		} 

		return begin + middle + end;
	}

	extern void Process(EXCEPTION_POINTERS* info) 
	try {
		HANDLE process = GetCurrentProcess();
		HANDLE thread = GetCurrentThread();

		DWORD machine = IMAGE_FILE_MACHINE_I386;
		CONTEXT context = {};
		memcpy(&context, info->ContextRecord, sizeof(CONTEXT));

		Safe_SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_DEFERRED_LOADS | SYMOPT_UNDNAME | SYMOPT_ALLOW_ABSOLUTE_SYMBOLS);

		char workingDirectory[MAX_PATH];
		char symbolPath[MAX_PATH];
		char altSymbolPath[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, workingDirectory);
		GetEnvironmentVariable("_NT_SYMBOL_PATH", symbolPath, MAX_PATH);
		GetEnvironmentVariable("_NT_ALTERNATE_SYMBOL_PATH ", altSymbolPath, MAX_PATH);
		std::string lookPath = std::format("{};{}\\Data\\NVSE\\Plugins;{};{}", workingDirectory, workingDirectory, symbolPath, altSymbolPath);

		//	SymSetExtendedOption((IMAGEHLP_EXTENDED_OPTIONS)SYMOPT_EX_WINE_NATIVE_MODULES, TRUE);
		if (!Safe_SymInitialize(process, lookPath.c_str(), true))
			output << "Error initializing symbol store" << '\n';

		//	SymSetExtendedOption((IMAGEHLP_EXTENDED_OPTIONS)SYMOPT_EX_WINE_NATIVE_MODULES, TRUE);

		STACKFRAME frame = {};
		frame.AddrPC.Offset = info->ContextRecord->Eip;
		frame.AddrPC.Mode = AddrModeFlat;
		frame.AddrFrame.Offset = info->ContextRecord->Ebp;
		frame.AddrFrame.Mode = AddrModeFlat;
		frame.AddrStack.Offset = info->ContextRecord->Esp;
		frame.AddrStack.Mode = AddrModeFlat;
		DWORD eip = 0;

		// retarded crutch to try to copy dbghelp before.
		output << "Calltrace:" << '\n' << std::format("{:^10} |  {:^40} | {:^40} | Source", "ebp", "Function Address", "Function Name") <<
			'\n';

		while (Safe_StackWalk(machine, process, thread, &frame, &context, NULL, Safe_SymFunctionTableAccess, Safe_SymGetModuleBase, NULL)) {
			/*
			Using  a PDB for OBSE from VS2019 is causing the frame to repeat, but apparently only if WINEDEBUG=+dbghelp isn't setted. Is this a wine issue?
			When this happen winedbg show only the first line (this happens with the first frame only probably, even if there are more frames shown when using WINEDEBUG=+dbghelp )
			*/
			if (frame.AddrPC.Offset == eip) break;
			eip = frame.AddrPC.Offset;
			output << GetCalltraceFunction(frame.AddrPC.Offset, frame.AddrFrame.Offset, process) << '\n';
		}
	}
	catch (...) {  output << "Failed to log callstack." << '\n'; }

	extern std::stringstream& Get() { output.flush(); return output; }
}
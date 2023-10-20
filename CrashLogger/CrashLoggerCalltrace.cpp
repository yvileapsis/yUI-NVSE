#include <CrashLogger.hpp>

namespace CrashLogger::Exception
{
	extern void Get(EXCEPTION_POINTERS* info)
	try { Log() << std::format("Exception {:08X} caught!\n", info->ExceptionRecord->ExceptionCode); }
	catch (...) { Log() << "Failed to log exception." << std::endl; }
}

namespace CrashLogger::Thread
{
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

	extern void Get(EXCEPTION_POINTERS* info)
	try { Log() << "Thread: " << GetThreadName() << std::endl; }
	catch (...) { Log() << "Failed to log thread name." << std::endl; }
}

namespace CrashLogger::Calltrace
{
	std::string GetCalltraceFunction(UInt32 eip, UInt32 ebp, HANDLE process)
	{
		/*if (GetModuleFileName((HMODULE)frame.AddrPC.Offset, path, MAX_PATH)) {  //Do this work on non base addresses even on  Windows? Cal directly the LDR function?
		if (!SymLoadModule(process, NULL, path, NULL, 0, 0)) Log() << FormatString("Porcoddio %0X", GetLastError());
		}*/

		const auto moduleBase = PDB::GetModuleBase(eip, process);

		std::string begin = std::format("0x{:08X} ==> ", ebp);

		std::string middle;

		const auto moduleOffset = (moduleBase != 0x00400000) ? eip - moduleBase + 0x10000000 : eip;

		if (const auto module = PDB::GetModule(eip, process); module.empty()) 
			middle = std::format("{:>20s} (0x{:08X}) : (Corrupt stack or heap?)", "-\\(°_o)/-", moduleOffset);
		else if (const auto symbol = PDB::GetSymbol(eip, process); symbol.empty())
			middle = std::format("{:>20s} (0x{:08X}) : EntryPoint+0xFFFFFFFF", module, moduleOffset);
		else
			middle = std::format("{:>20s} (0x{:08X}) : {}", module, moduleOffset, symbol);

		std::string end;

		if (const auto line = PDB::GetLine(eip, process); !line.empty())
		{
			middle = std::format("{:<80s}", middle);
			end = " <== " + line;
		} 

		return begin + middle + end;
	}

	extern void Get(EXCEPTION_POINTERS* info) 
	try {
		HANDLE process = GetCurrentProcess();
		HANDLE thread = GetCurrentThread();

		Log() << "Calltrace:";

		DWORD machine = IMAGE_FILE_MACHINE_I386;
		CONTEXT context = {};
		memcpy(&context, info->ContextRecord, sizeof(CONTEXT));

		Safe_SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_DEFERRED_LOADS | SYMOPT_UNDNAME | SYMOPT_ALLOW_ABSOLUTE_SYMBOLS);

		//    SymSetExtendedOption((IMAGEHLP_EXTENDED_OPTIONS)SYMOPT_EX_WINE_NATIVE_MODULES, TRUE);
		if (!Safe_SymInitialize(process, NULL, true))
			Log() << "Error initializing symbol store";

		//    SymSetExtendedOption((IMAGEHLP_EXTENDED_OPTIONS)SYMOPT_EX_WINE_NATIVE_MODULES, TRUE);

		STACKFRAME frame = {};
		frame.AddrPC.Offset = info->ContextRecord->Eip;
		frame.AddrPC.Mode = AddrModeFlat;
		frame.AddrFrame.Offset = info->ContextRecord->Ebp;
		frame.AddrFrame.Mode = AddrModeFlat;
		frame.AddrStack.Offset = info->ContextRecord->Esp;
		frame.AddrStack.Mode = AddrModeFlat;
		DWORD eip = 0;

		while (Safe_StackWalk(machine, process, thread, &frame, &context, NULL, Safe_SymFunctionTableAccess, Safe_SymGetModuleBase, NULL)) {
			/*
			Using  a PDB for OBSE from VS2019 is causing the frame to repeat, but apparently only if WINEDEBUG=+dbghelp isn't setted. Is this a wine issue?
			When this happen winedbg show only the first line (this happens with the first frame only probably, even if there are more frames shown when using WINEDEBUG=+dbghelp )
			*/
			if (frame.AddrPC.Offset == eip) break;
			eip = frame.AddrPC.Offset;
			Log() << GetCalltraceFunction(frame.AddrPC.Offset, frame.AddrFrame.Offset, process);
		}

		Log();
	}
	catch (...) {  Log() << "Failed to log callstack." << std::endl; }
}

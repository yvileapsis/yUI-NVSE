#include <CrashLogger.hpp>
#include <psapi.h>

namespace CrashLogger::Memory
{

	extern void Get(EXCEPTION_POINTERS* info)
	try 
	{	
		PROCESS_MEMORY_COUNTERS pmc;
	
		const auto hProcess = GetCurrentProcess();

		SIZE_T lpMinimumWorkingSetSize;
		SIZE_T lpMaximumWorkingSetSize;
		DWORD flags;

		Log() << "Memory:";

		if (GetProcessWorkingSetSizeEx(hProcess, &lpMinimumWorkingSetSize, &lpMaximumWorkingSetSize, &flags))
		{
			Log() << std::format("MinimumWorkingSetSize:      0x{:08X}", lpMinimumWorkingSetSize);
			Log() << std::format("MaximumWorkingSetSize:      0x{:08X}", lpMaximumWorkingSetSize);
			Log() << std::format("WorkingSetSizeFlags:        0x{:08X}", flags);
		}

		if ( GetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc)) )
		{
			Log() << std::format("PeakWorkingSetSize:         0x{:08X}", pmc.PeakWorkingSetSize);
			Log() << std::format("WorkingSetSize:             0x{:08X}", pmc.WorkingSetSize);
			Log() << std::format("QuotaPeakPagedPoolUsage:    0x{:08X}", pmc.QuotaPeakPagedPoolUsage);
			Log() << std::format("QuotaPagedPoolUsage:        0x{:08X}", pmc.QuotaPagedPoolUsage);
			Log() << std::format("QuotaPeakNonPagedPoolUsage: 0x{:08X}", pmc.QuotaPeakNonPagedPoolUsage);
			Log() << std::format("QuotaNonPagedPoolUsage:     0x{:08X}", pmc.QuotaNonPagedPoolUsage);
			Log() << std::format("PageFaultCount:             0x{:08X}", pmc.PageFaultCount);
			Log() << std::format("PagefileUsage:              0x{:08X}", pmc.PagefileUsage);
			Log() << std::format("PeakPagefileUsage:          0x{:08X}", pmc.PeakPagefileUsage);
		}

		Log();
	}
	catch (...) { Log() << "Failed to log memory." << std::endl; }
}
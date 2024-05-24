#include <CrashLogger.hpp>
#include <psapi.h>

namespace CrashLogger::Memory
{
	std::stringstream output;

	extern void Process(EXCEPTION_POINTERS* info)
	try 
	{
		const auto hProcess = GetCurrentProcess();

		output << "Memory:" << '\n';

		PROCESS_MEMORY_COUNTERS_EX2 pmc = {};
		pmc.cb = sizeof(pmc);

		// Get physical memory size
		MEMORYSTATUSEX memoryStatus;
		memoryStatus.dwLength = sizeof(memoryStatus);
		GlobalMemoryStatusEx(&memoryStatus);

		if ( GetProcessMemoryInfo( hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc)) )
		{
			float physPercentage = ConvertToMB(pmc.WorkingSetSize) / ConvertToMB(memoryStatus.ullTotalPhys) * 100.0f;
			float virtPercentage = ConvertToMB(pmc.PrivateUsage) / ConvertToMB(memoryStatus.ullTotalVirtual) * 100.0f;
			output << std::format("Physical Usage: {:10}/{:10} ({:.2f}%)", FormatSize(pmc.WorkingSetSize), FormatSize(memoryStatus.ullTotalPhys), physPercentage) <<
				'\n';
			output << std::format("Virtual  Usage: {:10}/{:10} ({:.2f}%)", FormatSize(pmc.PrivateUsage), FormatSize(memoryStatus.ullTotalVirtual), virtPercentage) <<
				'\n';
		}
	}
	catch (...) { output << "Failed to log memory." << '\n'; }

	extern std::stringstream& Get() { return output; }
}
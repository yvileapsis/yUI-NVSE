#include <CrashLogger.hpp>
#include <psapi.h>

namespace CrashLogger::Memory
{
	float ConvertToKB(SIZE_T size) {
		return (float)size / 1024.0f;
	}

	float ConvertToMB(SIZE_T size) {
		return (float)size / 1024.0f / 1024.0f;
	}

	float ConvertToGB(SIZE_T size) {
		return (float)size / 1024.0f / 1024.0f / 1024.0f;
	}

	std::string FormatSize(SIZE_T size) {
		std::string result;
		if (size < 1024) {
			result = std::format("{:d} B", size);
		}
		else if (size < 1024 * 1024) {
			result = std::format("{:.2f} KB", ConvertToKB(size));
		}
		else if (size < 1024 * 1024 * 1024) {
			result = std::format("{:.2f} MB", ConvertToMB(size));
		}
		else {
			result = std::format("{:.2f} GB", ConvertToGB(size));
		}
		return result;
	}

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
			Log() << std::format("MinimumWorkingSetSize:      {}", FormatSize(lpMinimumWorkingSetSize));
			Log() << std::format("MaximumWorkingSetSize:      {}", FormatSize(lpMaximumWorkingSetSize));
		}

		if ( GetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc)) )
		{
			Log() << std::format("PeakWorkingSetSize:         {}", FormatSize(pmc.PeakWorkingSetSize));
			Log() << std::format("WorkingSetSize:             {}", FormatSize(pmc.WorkingSetSize));
			Log() << std::format("QuotaPeakPagedPoolUsage:    {}", FormatSize(pmc.QuotaPeakPagedPoolUsage));
			Log() << std::format("QuotaPagedPoolUsage:        {}", FormatSize(pmc.QuotaPagedPoolUsage));
			Log() << std::format("QuotaPeakNonPagedPoolUsage: {}", FormatSize(pmc.QuotaPeakNonPagedPoolUsage));
			Log() << std::format("QuotaNonPagedPoolUsage:     {}", FormatSize(pmc.QuotaNonPagedPoolUsage));
			Log() << std::format("PageFaultCount:             {}", FormatSize(pmc.PageFaultCount));
			Log() << std::format("PagefileUsage:              {}", FormatSize(pmc.PagefileUsage));
			Log() << std::format("PeakPagefileUsage:          {}", FormatSize(pmc.PeakPagefileUsage));
		}

		Log();
	}
	catch (...) { Log() << "Failed to log memory." << std::endl; }
}
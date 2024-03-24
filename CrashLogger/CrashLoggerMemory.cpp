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
			result = std::format("{:>6d} B", size);
		}
		else if (size < 1024 * 1024) {
			result = std::format("{:>6.2f} KB", ConvertToKB(size));
		}
		else if (size < 1024 * 1024 * 1024) {
			result = std::format("{:>6.2f} MB", ConvertToMB(size));
		}
		else {
			result = std::format("{:>6.2f} GB", ConvertToGB(size));
		}
		return result;
	}

	extern void Get(EXCEPTION_POINTERS* info)
	try 
	{	

	
		const auto hProcess = GetCurrentProcess();

		Log() << "Memory:";

		PROCESS_MEMORY_COUNTERS_EX2 pmc = {};
		pmc.cb = sizeof(pmc);

		if ( GetProcessMemoryInfo( hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc)) )
		{
			Log() << std::format("Physical Usage: {:10}", FormatSize(pmc.WorkingSetSize));
			Log() << std::format("Virtual  Usage: {:10}", FormatSize(pmc.PrivateUsage));
		}

		Log();
	}
	catch (...) { Log() << "Failed to log memory." << std::endl; }
}
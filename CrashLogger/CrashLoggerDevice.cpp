#include <CrashLogger.hpp>

namespace CrashLogger::Device
{

	std::string GetRegistryString(HKEY key, const char* name)
	{
		char buffer[MAX_PATH];
		DWORD size = sizeof(buffer);
		if (RegQueryValueExA(key, name, nullptr, nullptr, (BYTE*)buffer, &size) == ERROR_SUCCESS)
			return buffer;
		return "Unknown";
	}

	extern void Get(EXCEPTION_POINTERS* info)
		try
	{

		Log() << "Device:";

		const char* gpu = *(const char**)0x11C72C4;
		std::string cpu = "Unknown";

		{
			HKEY key;
			if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &key) == ERROR_SUCCESS)
			{
				cpu = GetRegistryString(key, "ProcessorNameString");
				RegCloseKey(key);
			}
		}

		std::string version;
		std::string buildNumber;
		std::string release;
		{
			HKEY key;
			if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", 0, KEY_READ, &key) == ERROR_SUCCESS) {
				release = GetRegistryString(key, "DisplayVersion");

				buildNumber = GetRegistryString(key, "CurrentBuild");

				version = GetRegistryString(key, "ProductName");

				UInt32 buildNumberInt = std::stoul(buildNumber);
				if (buildNumberInt >= 22000) {
					// Aaahhh i'm a schizo
					version.replace(9, 1, "1");
				}

				RegCloseKey(key);
			}
		}

		ULONGLONG memAmount;
		GetPhysicallyInstalledSystemMemory(&memAmount);
		
		Log() << std::format("OS:  \"{} - {} ({})\"", version, buildNumber, release);
		Log() << std::format("CPU: \"{}\"", cpu);
		Log() << std::format("GPU: {}", gpu);
		Log() << std::format("RAM: \"{:>5.2f} GB\"", memAmount / 1024.f / 1024.f);

		Log();
	}
	catch (...) { Log() << "Failed to print device info." << std::endl; }
}
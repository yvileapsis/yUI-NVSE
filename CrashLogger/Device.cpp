#include <CrashLogger.hpp>

namespace CrashLogger::Device
{
	std::stringstream output;

	bool GetRegistryString(HKEY key, const char* name, char* buffer, SIZE_T bufferSize)
	{
		return RegQueryValueExA(key, name, nullptr, nullptr, (BYTE*)buffer, &bufferSize) == ERROR_SUCCESS;
	}

	extern void Process(EXCEPTION_POINTERS* info)
	try
	{

		output << "Device\n";

		const char* gpu = *(const char**)0x11C72C4;
		char cCPU[128] = {};

		{
			HKEY key;
			if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &key) == ERROR_SUCCESS)
			{
				GetRegistryString(key, "ProcessorNameString", cCPU, sizeof(cCPU));
				RegCloseKey(key);
			}
		}

		char version[64];
		char buildNumber[8];
		char release[16];
		{
			HKEY key;
			if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", 0, KEY_READ, &key) == ERROR_SUCCESS) {
				GetRegistryString(key, "DisplayVersion", release, sizeof(release));

				GetRegistryString(key, "CurrentBuild", buildNumber, sizeof(buildNumber));

				GetRegistryString(key, "ProductName", version, sizeof(version));

				UInt32 buildNumberInt = std::stoul(buildNumber);
				if (buildNumberInt >= 22000) {
					// Aaahhh i'm a schizo (context: Windows 11 still reports itself as Windows 10 here)
					char* p = strstr(version, "10");
					if (p) {
						p[0] = '1';
						p[1] = '1';
					}
				}

				RegCloseKey(key);
			}
		}

		ULONGLONG memAmount;
		GetPhysicallyInstalledSystemMemory(&memAmount);

		// Trim the empty space at the end of the CPU string
		for (int i = sizeof(cCPU) - 1; i >= 0; i--) {
			if (cCPU[i] == ' ')
				cCPU[i] = '\0';
			else
				break;
		}

		char textBuffer[512];
		
		sprintf_s(textBuffer, "OS:  \"%s - %s (%s)\"\n", version, buildNumber, release);
		output << textBuffer;

		sprintf_s(textBuffer, "CPU: \"%s\"\n", cCPU);
		output << textBuffer;

		sprintf_s(textBuffer, "GPU: %s\n", gpu); // Already has quotes
		output << textBuffer;

		sprintf_s(textBuffer, "RAM: \"%.2f GB\"\n", memAmount / 1024.f / 1024.f);
		output << textBuffer;

	}
	catch (...) { output << "Failed to print device info.\n"; }

	extern std::stringstream& Get() { output.flush(); return output; }
}
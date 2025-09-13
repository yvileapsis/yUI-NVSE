#include <CrashLogger.hpp>

#pragma comment (lib, "version.lib")

namespace CrashLogger::Modules
{
	std::stringstream output;

	struct UserContext {
		UInt32 eip;
		UInt32 moduleBase;
		CHAR* name;
	};

	struct Module
	{
		UInt32 moduleBase;
		UInt32 moduleEnd;
		std::filesystem::path path;

		bool operator<(const Module& other) const {
			if (path < other.path) return true;
			return false;
		};
	};

	std::set<Module> enumeratedModules;

	BOOL CALLBACK EumerateModulesCallback(PCSTR name, ULONG moduleBase, ULONG moduleSize, PVOID context) {
		UserContext* info = (UserContext*)context;
		if (info->eip >= (UInt32)moduleBase && info->eip <= (UInt32)moduleBase + (UInt32)moduleSize) {
			info->moduleBase = moduleBase;
			strcpy_s(info->name, 100, name);
		}
		enumeratedModules.emplace((UInt32)moduleBase, (UInt32)moduleBase + (UInt32)moduleSize, std::string(name));

		return TRUE;
	}

	static std::unordered_map<std::string, const char*> pluginNames = {
		{"hot_reload_editor", "hot_reload"},
		{"ilsfix", "ILS Fix"},
		{"improved_console", "Improved Console"},
		{"ImprovedLightingShaders", "Improved Lighting Shaders for FNV"},
		{"jip_nvse", "JIP LN NVSE"},
		{"johnnyguitar", "JohnnyGuitarNVSE"},
		{"MCM", "MCM Extensions"},
		{"mod_limit_fix", "Mod Limit Fix"},
		{"nvse_console_clipboard", "Console Clipboard"},
		{"nvse_stewie_tweaks", "lStewieAl's Tweaks"},
		{"ShowOffNVSE", "ShowOffNVSE Plugin"},
		{"supNVSE", "SUP NVSE Plugin"},
		{"ui_organizer", "UI Organizer Plugin"},
		{"EngineOptimizations", "Engine Optimizations"},
		{"DynamicReflections", "Dynamic Reflections"},
		{"Alpha Fixes", "Fallout Alpha Rendering Tweaks"}
	};

	std::string GetPluginNameForFileName(std::string name)
	{
		if (pluginNames.contains(name))
			return pluginNames[name];
		return name;
	}

	std::string GetFileVersion(std::string path) {
		char version[64] = { 0 };
		DWORD infoSize = GetFileVersionInfoSizeA(path.c_str(), 0);
		LPSTR verData = new char[infoSize];
		if (GetFileVersionInfo(path.c_str(), 0, infoSize, verData)) {
			LPBYTE lpBuffer = nullptr;
			UINT size = 0;
			if (VerQueryValue(verData, "\\", (VOID FAR * FAR*) & lpBuffer, &size) && size) {
				VS_FIXEDFILEINFO* verInfo = (VS_FIXEDFILEINFO*)lpBuffer;
				if (verInfo->dwSignature == 0xfeef04bd) {
					sprintf_s(version, "%d.%d.%d.%d", verInfo->dwFileVersionMS >> 16, verInfo->dwFileVersionMS & 0xffff, verInfo->dwFileVersionLS >> 16, verInfo->dwFileVersionLS & 0xffff);
				}
			}
		}
		delete[] verData;
		return std::string(version);
	}

	extern void Process(EXCEPTION_POINTERS* info)
	try {
		HANDLE process = GetCurrentProcess();

		const UInt32 eip = info->ContextRecord->Eip;

		UserContext infoUser = { eip,  0, (char*)calloc(sizeof(char), 100) };

		Safe_EnumerateLoadedModules(process, EumerateModulesCallback, &infoUser);

		size_t memoryAllocated = 0;

		char textBuffer[512];
		sprintf_s(textBuffer, "Module bases:\n %*s%*s  | %*s%*s | %*s%*s | %*s\n", CENTERED_TEXT(22, "Address"), CENTERED_TEXT(40, "Module"), CENTERED_TEXT(20, "Version"), 40, "Filepath");
		output << textBuffer;
		for (const auto& [moduleBase, moduleEnd, path] : enumeratedModules)
		{
			char version[64] = {};

			if (g_commandInterface) {
				if (g_commandInterface->version >= 2) {
					const PluginInfo* info = g_commandInterface->GetPluginInfoByDLLName(path.filename().generic_string().c_str());
					if (info) {
						sprintf_s(version, "%d", info->version);
					}
				}
				else if (const auto info = g_commandInterface->GetPluginInfoByName(GetPluginNameForFileName(path.stem().generic_string()).c_str())) {
					sprintf_s(version, "%d", info->version);
				}
			}

			if (version[0] == 0) {
				const std::string& dll_version = GetFileVersion(path.generic_string());
				if (dll_version.empty())
					strcpy_s(version, "Unknown");
				else
					strcpy_s(version, dll_version.c_str());
			}

			memoryAllocated += moduleEnd - moduleBase;

			sprintf_s(textBuffer, " 0x%08X - 0x%08X | %-40s | %-20s | %s\n", moduleBase, moduleEnd, path.stem().generic_string().c_str(), version, SanitizeString(path.generic_string()).c_str());
			output << textBuffer;
		}

		char cMemBuffer[128];
		FormatSize(memoryAllocated, cMemBuffer, sizeof(cMemBuffer));
		sprintf_s(textBuffer, "\nTotal memory allocated to modules: %s\n", cMemBuffer);
		output << textBuffer;
	}
	catch (...) { output << "Failed to print out modules.\n"; }

	extern std::stringstream& Get() { output.flush(); return output; }
}
#include <CrashLogger.hpp>

#pragma comment (lib, "version.lib")

namespace CrashLogger::Modules
{
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

	static std::unordered_map<const char*, const char*> pluginNames = {
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

	const char* __fastcall GetPluginNameForFileName(const char* name)
	{
		if (pluginNames.contains(name))
			return pluginNames[name];
		return name;
	}

	const char* __fastcall GetFileVersion(std::string path, char* buffer, size_t bufferSize) {
		DWORD infoSize = GetFileVersionInfoSizeA(path.c_str(), 0);
		LPSTR verData = new char[infoSize];
		if (GetFileVersionInfo(path.c_str(), 0, infoSize, verData)) {
			LPBYTE lpBuffer = nullptr;
			UINT size = 0;
			if (VerQueryValue(verData, "\\", (VOID FAR * FAR*) & lpBuffer, &size) && size) {
				VS_FIXEDFILEINFO* verInfo = (VS_FIXEDFILEINFO*)lpBuffer;
				if (verInfo->dwSignature == 0xfeef04bd) {
					sprintf_s(buffer, bufferSize, "%d.%d.%d.%d", verInfo->dwFileVersionMS >> 16, verInfo->dwFileVersionMS & 0xffff, verInfo->dwFileVersionLS >> 16, verInfo->dwFileVersionLS & 0xffff);
				}
			}
		}
		delete[] verData;
		return buffer;
	}

	extern void __fastcall Process(EXCEPTION_POINTERS* info)
	try {
		HANDLE process = GetCurrentProcess();

		const UInt32 eip = info->ContextRecord->Eip;

		UserContext infoUser = { eip,  0, (char*)calloc(sizeof(char), 100) };

		Safe_EnumerateLoadedModules(process, EumerateModulesCallback, &infoUser);

		size_t memoryAllocated = 0;

		_MESSAGE("\nModule bases:\n %*s%*s  | %*s%*s | %*s%*s | %*s", CENTERED_TEXT(22, "Address"), CENTERED_TEXT(40, "Module"), CENTERED_TEXT(20, "Version"), 40, "Filepath");
		for (const auto& [moduleBase, moduleEnd, path] : enumeratedModules)
		{
			char version[64] = {};

			if (g_commandInterface) {
				if (g_commandInterface->version >= 2) {
					if (const PluginInfo* info = g_commandInterface->GetPluginInfoByDLLName(path.filename().generic_string().c_str())) {
						sprintf_s(version, "%d", info->version);
					}
				}
				else if (const auto info = g_commandInterface->GetPluginInfoByName(GetPluginNameForFileName(path.stem().generic_string().c_str()))) {
					sprintf_s(version, "%d", info->version);
				}
			}

			if (version[0] == 0) {
				GetFileVersion(path.generic_string(), version, sizeof(version));
				if (version[0] == 0)
					strcpy_s(version, "Unknown");
			}

			memoryAllocated += moduleEnd - moduleBase;

			char sanitizedPath[MAX_PATH] = {};
			_MESSAGE(" 0x%08X - 0x%08X | %-40s | %-20s | %s", moduleBase, moduleEnd, path.stem().generic_string().c_str(), version, SanitizeString(path.generic_string().c_str(), sanitizedPath, sizeof(sanitizedPath)));
		}

		char cMemBuffer[32];
		FormatSize(memoryAllocated, cMemBuffer, sizeof(cMemBuffer));
		_MESSAGE("\nTotal memory allocated to modules: %s", cMemBuffer);
	}
	catch (...) { _MESSAGE("Failed to print out modules."); }
}
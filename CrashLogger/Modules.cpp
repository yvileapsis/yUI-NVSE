#include <CrashLogger.hpp>
#include <tchar.h>
#include <strsafe.h>

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
		size_t moduleBase;
		size_t moduleEnd;
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
		enumeratedModules.emplace(moduleBase, moduleBase + moduleSize, name);

		return TRUE;
	}

	static std::unordered_map<std::string_view, const char*> pluginNames = {
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

	void __fastcall GetFileVersion(const char* apPath, char* apVerBuffer, size_t apVerBufferSize) {
		DWORD dwInfoSize = GetFileVersionInfoSize(apPath, 0);
		if (dwInfoSize) {
			char* pData = new char[dwInfoSize];
			if (GetFileVersionInfo(apPath, 0, dwInfoSize, pData)) {
				if (apVerBuffer) {
					void* pFileInfoRaw = nullptr;
					uint32_t uiInfoSize = 0;
					if (VerQueryValue(pData, "\\", &pFileInfoRaw, &uiInfoSize) && uiInfoSize) {
						VS_FIXEDFILEINFO* pFileInfo = static_cast<VS_FIXEDFILEINFO*>(pFileInfoRaw);
						if (pFileInfo->dwSignature == 0xfeef04bd)
							sprintf_s(apVerBuffer, apVerBufferSize, "%d.%d.%d.%d", pFileInfo->dwFileVersionMS >> 16, pFileInfo->dwFileVersionMS & 0xffff, pFileInfo->dwFileVersionLS >> 16, pFileInfo->dwFileVersionLS & 0xffff);
					}
				}
			}
			delete[] pData;
		}
	}

	extern void __fastcall Process(EXCEPTION_POINTERS* info)
	try {
		HANDLE process = GetCurrentProcess();

		const UInt32 eip = info->ContextRecord->Eip;

		UserContext infoUser = { eip,  0, (char*)calloc(sizeof(char), 100) };

		Safe_EnumerateLoadedModules(process, EumerateModulesCallback, &infoUser);

		size_t memoryAllocated = 0;
		size_t memoryAllocatedNVSE = 0;

		_MESSAGE("\nModule bases:\n %*s%*s  | %*s%*s | %*s%*s | %*s%*s | %*s", CENTERED_TEXT(22, "Address Range"), CENTERED_TEXT(10, "Size"), CENTERED_TEXT(40, "Module"), CENTERED_TEXT(20, "Version"), 40, "Filepath");
		for (const auto& [moduleBase, moduleEnd, path] : enumeratedModules) {
			size_t stSize = moduleEnd - moduleBase;
			memoryAllocated += stSize;

			char version[64] = {};
			const char* pDesc = nullptr;

			if (g_commandInterface) {
				if (g_commandInterface->version >= 2) {
					if (const PluginInfo* info = g_commandInterface->GetPluginInfoByDLLName(path.filename().generic_string().c_str())) {
						sprintf_s(version, "%d", info->version);
						pDesc = info->name;
						memoryAllocatedNVSE += stSize;
					}
				}
				else if (const auto info = g_commandInterface->GetPluginInfoByName(GetPluginNameForFileName(path.stem().generic_string().c_str()))) {
					sprintf_s(version, "%d", info->version);
					pDesc = info->name;
					memoryAllocatedNVSE += stSize;
				}
			}

			if (version[0] == 0) {
				GetFileVersion(path.generic_string().c_str(), version, sizeof(version));
				if (version[0] == 0)
					strcpy_s(version, "Unknown");
			}

			char sanitizedPath[MAX_PATH] = {};
			char sizeBuffer[32] = {};
			_MESSAGE(" 0x%08X - 0x%08X | %-10s | %-40s | %-20s | %s", moduleBase, moduleEnd, FormatSize(stSize, sizeBuffer, sizeof(sizeBuffer)), pDesc ? pDesc : path.stem().generic_string().c_str(), version, SanitizeString(path.generic_string().c_str(), sanitizedPath, sizeof(sanitizedPath)));
		}

		char cMemBuffer[32];
		FormatSize(memoryAllocated, cMemBuffer, sizeof(cMemBuffer));
		_MESSAGE("\nTotal memory allocated to all modules:  %-10s", cMemBuffer);
		FormatSize(memoryAllocatedNVSE, cMemBuffer, sizeof(cMemBuffer));
		_MESSAGE("Total memory allocated to NVSE plugins: %-10s", cMemBuffer);

		free(infoUser.name);
	}
	catch (...) { _MESSAGE("Failed to print out modules."); }
}
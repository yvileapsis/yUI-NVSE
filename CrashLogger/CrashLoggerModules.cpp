#include <CrashLogger.hpp>

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
			//Log() << FormatString("%0X %0X  %0X", (UInt32)moduleBase, info->eip, (UInt32)moduleBase + (UInt32) moduleSize);
			info->moduleBase = moduleBase;
			strcpy_s(info->name, 100, name);
		}

		enumeratedModules.emplace((UInt32)moduleBase, (UInt32)moduleBase + (UInt32)moduleSize, std::string(name));

		return TRUE;
	}

	std::unordered_map<std::string, std::string> pluginNames = {
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

	extern void Get(EXCEPTION_POINTERS* info)
	try {
		HANDLE process = GetCurrentProcess();

		const UInt32 eip = info->ContextRecord->Eip;

		UserContext infoUser = { eip,  0, (char*)calloc(sizeof(char), 100) };

		Safe_EnumerateLoadedModules(process, EumerateModulesCallback, &infoUser);

		Log() << "Module bases:";
		for (const auto& [moduleBase, moduleEnd, path] : enumeratedModules)
		{
			std::string version;

			if (g_commandInterface) if (const auto info = g_commandInterface->GetPluginInfoByName(GetPluginNameForFileName(path.stem().generic_string()).c_str()))
				version = std::format("NVSE plugin version: {:>4d}, ", info->version);

			Log() << std::format("0x{:08X} - 0x{:08X} ==> {:25s}{:>30s}{}", moduleBase, moduleEnd, path.stem().generic_string() + ",", version, SanitizeString(path.generic_string()));

		}

		Log();

		if (infoUser.moduleBase)
			Log() << std::format("GAME CRASHED AT INSTRUCTION Base+0x{:08X} IN MODULE: {}", (infoUser.eip - infoUser.moduleBase), infoUser.name) << std::endl
			<< "Please note that this does not automatically mean that that module is responsible. It may have been supplied bad data or" << std::endl
			<< "program state as the result of an issue in the base game or a different DLL.";
		else
			Log() << "UNABLE TO IDENTIFY MODULE CONTAINING THE CRASH ADDRESS." << std::endl
			<< "This can occur if the crashing instruction is located in the vanilla address space, but it can also occur if there are too many" << std::endl
			<< "DLLs for us to list, and if the crash occurred in one of their address spaces. Please note that even if the crash occurred" << std::endl
			<< "in vanilla code, that does not necessarily mean that it is a vanilla problem. The vanilla code may have been supplied bad data" << std::endl
			<< "or program state as the result of an issue in a loaded DLL.";

		Log();
	}
	catch (...) { Log() << "Failed to print out modules." << std::endl; }
}

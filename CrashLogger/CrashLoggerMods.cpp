#include <CrashLogger.hpp>
#include "TESDataHandler.hpp"
#include <filesystem>

namespace CrashLogger::Mods
{
	extern void Get(EXCEPTION_POINTERS* info)
	try {

		Log() << "Install: " << SanitizeString(GetFalloutDirectory().generic_string()) << std::endl;

		Log() << "Mods:" << std::endl << std::format(" # | {:^80} | {:^40} |", "Mod", "Author");
		for (UInt32 i = 0; i < g_TESDataHandler->kMods.uiLoadedModCount; i++) {
			const auto mod = g_TESDataHandler->kMods.pLoadedMods[i];
			if (!mod)
				continue;

			const auto author = mod->author.StdStr();
			if (author.empty() || !author.compare("DEFAULT"))
				Log() << std::format("{:02X} | {:80} | {:40} |", i, mod->m_Filename, "");
			else
				Log() << std::format("{:02X} | {:80} | {:40} |", i, mod->m_Filename, author);
		}

		std::string folder_path = std::format("{}data/nvse/plugins/scripts", GetFalloutDirectory().generic_string());

		if (std::filesystem::exists(folder_path) && std::filesystem::is_directory(folder_path)) {
			Log() << std::endl;
			Log() << std::format("Script Runners:");

			// Iterate through each entry in the directory
			for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {
				if (entry.path().extension().string()._Equal(".txt")) {
					Log() << entry.path().filename();
				}
			}
		}

		Log();
	}
	catch (...) { Log() << "Failed to print out mod list." << std::endl; }
}
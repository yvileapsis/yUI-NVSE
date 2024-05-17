#include <CrashLogger.hpp>
#include "TESDataHandler.hpp"
#include <filesystem>

namespace CrashLogger::Install
{
	std::stringstream output;

	extern void Process(EXCEPTION_POINTERS* info)
	try {
		output << "Install: " << SanitizeString(GetFalloutDirectory().generic_string()) << std::endl;
	}
	catch (...) { output << "Failed to print install path." << std::endl; }

	extern std::stringstream& Get() { return output; }
}

namespace CrashLogger::Mods
{
	std::stringstream output;

	extern void Process(EXCEPTION_POINTERS* info)
	try {
		output << "Mods:" << std::endl << std::format(" # | {:^80} | {:^40} |", "Mod", "Author") << std::endl;
		for (UInt32 i = 0; i < g_TESDataHandler->kMods.uiLoadedModCount; i++) {
			const auto mod = g_TESDataHandler->kMods.pLoadedMods[i];
			if (!mod)
				continue;

			const auto author = mod->author.StdStr();
			if (author.empty() || !author.compare("DEFAULT"))
				output << std::format("{:02X} | {:80} | {:40} |", i, mod->m_Filename, "") << std::endl;
			else
				output << std::format("{:02X} | {:80} | {:40} |", i, mod->m_Filename, author) << std::endl;
		}
		output << std::endl;

		std::string folder_path = std::format("{}data/nvse/plugins/scripts", GetFalloutDirectory().generic_string());

		if (std::filesystem::exists(folder_path) && std::filesystem::is_directory(folder_path)) {
			output << std::format("Script Runners:") << std::endl;

			// Iterate through each entry in the directory
			for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {
				if (entry.path().extension().string()._Equal(".txt")) {
					output << entry.path().filename() << std::endl;
				}
			}
		}
	}
	catch (...) { output << "Failed to print out mod list." << std::endl; }

	extern std::stringstream& Get() { return output; }
}
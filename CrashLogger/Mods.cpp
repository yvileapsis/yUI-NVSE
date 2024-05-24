#include <CrashLogger.hpp>
#include "TESDataHandler.hpp"
#include <filesystem>

namespace CrashLogger::Install
{
	std::stringstream output;

	extern void Process(EXCEPTION_POINTERS* info)
	try {
		output << "Install: " << SanitizeString(GetFalloutDirectory().generic_string()) << '\n';
	}
	catch (...) { output << "Failed to print install path." << '\n'; }

	extern std::stringstream& Get() { return output; }
}

namespace CrashLogger::Mods
{
	std::stringstream output;

	extern void Process(EXCEPTION_POINTERS* info)
	try {
		output << "Mods:" << '\n' << std::format(" # | {:^80} | {:^40} |", "Mod", "Author") << '\n';
		for (UInt32 i = 0; i < g_TESDataHandler->kMods.uiLoadedModCount; i++) {
			const auto mod = g_TESDataHandler->kMods.pLoadedMods[i];
			if (!mod)
				continue;

			const auto author = mod->author.StdStr();
			if (author.empty() || !author.compare("DEFAULT"))
				output << std::format("{:02X} | {:80} | {:40} |", i, mod->m_Filename, "") << '\n';
			else
				output << std::format("{:02X} | {:80} | {:40} |", i, mod->m_Filename, author) << '\n';
		}
		output << '\n';

		std::string folder_path = std::format("{}data/nvse/plugins/scripts", GetFalloutDirectory().generic_string());

		if (std::filesystem::exists(folder_path) && std::filesystem::is_directory(folder_path)) {
			output << std::format("Script Runners:") << '\n';

			// Iterate through each entry in the directory
			for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {
				if (entry.path().extension().string()._Equal(".txt")) {
					output << entry.path().filename() << '\n';
				}
			}
		}
	}
	catch (...) { output << "Failed to print out mod list." << '\n'; }

	extern std::stringstream& Get() { return output; }
}
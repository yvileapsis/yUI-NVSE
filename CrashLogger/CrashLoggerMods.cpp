#include <CrashLogger.hpp>
#include <GameData.h>
#include <filesystem>

namespace CrashLogger::Mods
{
	extern void Get(EXCEPTION_POINTERS* info)
	try {
		Log() << "Mods:";
		for (UInt32 i = 0; i < g_TESDataHandler->modList.loadedModCount; i++) {
			ModInfo* mod = g_TESDataHandler->modList.loadedMods[i];
			if (!mod)
				continue;

			const char* author = mod->author.CStr();
			if (!author || !author[0] || !strcmp(author, "DEFAULT")) {
				Log() << std::format("{:02X} - {:60}", i, mod->name).c_str();
			}
			else {
				Log() << std::format("{:02X} - {:60} by {:40}", i, mod->name, author).c_str();
			}
		}


		std::string folder_path = std::format("{}data/nvse/plugins/scripts", GetFalloutDirectory().generic_string());

		if (std::filesystem::exists(folder_path) && std::filesystem::is_directory(folder_path)) {
			Log() << std::endl;
			Log() << std::format("Sctrptrunners:");

			// Iterate through each entry in the directory
			for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {
				if (entry.path().extension().string()._Equal(".txt")) {
					Log() << entry.path().filename();
				}
			}
		}

		Log() << std::endl;
	}
	catch (...) { Log() << "Failed to print out mod list." << std::endl; }
}
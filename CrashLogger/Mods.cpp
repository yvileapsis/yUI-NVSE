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

	extern std::stringstream& Get() { output.flush(); return output; }
}

namespace CrashLogger::Mods
{
	std::stringstream output;

	extern void Process(EXCEPTION_POINTERS* info)
	try {
		char textBuffer[512];
		sprintf_s(textBuffer, "Mods:\n  # | %*s%*s | %*s%*s\n", CENTERED_TEXT(80, "Mod"), CENTERED_TEXT(60, "Author"));
		output << textBuffer;
		for (UInt32 i = 0; i < g_TESDataHandler->kMods.uiLoadedModCount; i++) {
			const auto mod = g_TESDataHandler->kMods.pLoadedMods[i];
			if (!mod)
				continue;

			const auto& author = mod->author;
			const char* authorName = author.pcString;
			if (!author.usLen || !strcmp(mod->author.pcString, "DEFAULT"))
				authorName = "";

			sprintf_s(textBuffer, " %02X | %-80s | %-60s\n", i, mod->m_Filename, authorName);
			output << textBuffer;
		}
		output << '\n';

		char folder_path[MAX_PATH];
		sprintf_s(folder_path, "%s\\data\\nvse\\plugins\\scripts", GetFalloutDirectory().generic_string().c_str());

		if (std::filesystem::exists(folder_path) && std::filesystem::is_directory(folder_path)) {
			sprintf_s(textBuffer, "Script Runners:\n  # | %*s%*s\n", CENTERED_TEXT(80, "Filename"));
			output << textBuffer;

			UInt32 i = 0;
			// Iterate through each entry in the directory
			for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {
				if (entry.path().extension().string()._Equal(".txt")) {
					sprintf_s(textBuffer, " %02X | %-80s\n", i, entry.path().filename().string().c_str());
					output << textBuffer;
					i++;
				}
			}
		}
	}
	catch (...) { output << "Failed to print out mod list." << '\n'; }

	extern std::stringstream& Get() { output.flush(); return output; }
}
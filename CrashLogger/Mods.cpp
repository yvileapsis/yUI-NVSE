#include <CrashLogger.hpp>
#include "TESDataHandler.hpp"
#include <filesystem>

#include <shlwapi.h>

#pragma comment(lib, "Shlwapi.lib")

namespace CrashLogger::Install
{
	extern void __fastcall Process(EXCEPTION_POINTERS* info) {
		try {
			char sanitizedBuffer[MAX_PATH] = {};
			_MESSAGE("\nInstall: %s", SanitizeString(GetFalloutDirectory().generic_string().c_str(), sanitizedBuffer, sizeof(sanitizedBuffer)));
		}
		catch (...) { 
			_MESSAGE("Failed to print install path."); 
		}
	}
}

namespace CrashLogger::Mods
{
	extern void __fastcall Process(EXCEPTION_POINTERS* info) {
		try {
			_MESSAGE("\nMods:\n  # | %*s%*s | %*s%*s", CENTERED_TEXT(80, "Mod"), CENTERED_TEXT(60, "Author"));
			for (UInt32 i = 0; i < TESDataHandler::GetSingleton()->kMods.uiLoadedModCount; i++) {
				const auto mod = TESDataHandler::GetSingleton()->kMods.pLoadedMods[i];
				if (!mod)
					continue;

				const auto& author = mod->author;
				const char* authorName = author.pcString;
				if (!author.usLen || !strcmp(mod->author.pcString, "DEFAULT"))
					authorName = "";

				_MESSAGE(" %02X | %-80s | %-60s", i, mod->m_Filename, authorName);
			}

			char folder_path[MAX_PATH];
			sprintf_s(folder_path, "%s\\data\\nvse\\plugins\\scripts", GetFalloutDirectory().generic_string().c_str());
			if (PathIsDirectory(folder_path)) {
				_MESSAGE("\nScript Runners:\n  # | %*s%*s", CENTERED_TEXT(80, "Filename"));

				UInt32 i = 0;
				// Iterate through each entry in the directory
				WIN32_FIND_DATA findFileData;
				HANDLE hFind = INVALID_HANDLE_VALUE;
				char searchPath[MAX_PATH];
				sprintf_s(searchPath, "%s\\*.txt", folder_path);
				hFind = FindFirstFile(searchPath, &findFileData);
				if (hFind != INVALID_HANDLE_VALUE) {
					do {
						_MESSAGE(" %02X | %-80s", i, findFileData.cFileName);
						i++;
					} while (FindNextFile(hFind, &findFileData) != 0);
					FindClose(hFind);
				}
			}
		}
		catch (...) {
			_MESSAGE("\nFailed to print out mod list.");
		}
	}
}
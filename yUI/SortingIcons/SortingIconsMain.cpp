#include "SortingIcons.h"

#include <main.h>
#include <functions.h>
#include <SimpleINILibrary.h>

namespace SortingIcons
{
	void HandleINI()
	{
		CSimpleIniA ini;
		ini.SetUnicode();

		const auto iniPath = GetCurPath() + yUI_INI;
		if (ini.LoadFile(iniPath.c_str()) == SI_FILE) return;

		enable = ini.GetOrCreate("General", "bSortingIcons", true, "; enable 'Sorting and Icons' feature. If required files are not found this will do nothing.");

		logLevel = (LogLevel) ini.GetOrCreate("Sorting and Icons", "iDebug", 3, "; debug output for Sorting and Icons, useful for developers");

		bSort = ini.GetOrCreate("Sorting and Icons", "bSortInventory", 1, "; sort inventory according to tag names supplied in .json");
		bCategories = ini.GetOrCreate("Sorting and Icons", "bEnableCategories", 1, "; enable keyring-like clickable categories (this destroys vanilla keyring, so you have to have .json files supplying a new keyring category, i.e. ySI.json)");

		bIcons = ini.GetOrCreate("Sorting and Icons", "bAddIconsToInventory", 1, "; add ycons to inventory, container and barter menus");
		bPrompt = ini.GetOrCreate("Sorting and Icons", "bAddIconsToPrompt", 1, "; add ycons to interaction prompt");
		bHotkeys = ini.GetOrCreate("Sorting and Icons", "bReplaceHotkeyIcons", 1, "; replace hotkey icons with ycons");

		ini.SaveFile(iniPath.c_str(), false);
	}

	void ProcessEntries()
	{
		ra::sort(g_Items, [&](const std::unique_ptr<Item>& entry1, const std::unique_ptr<Item>& entry2) { return entry1->priority > entry2->priority; });
		ra::sort(g_Icons, [&](const std::unique_ptr<Icon>& entry1, const std::unique_ptr<Icon>& entry2) { return entry1->priority > entry2->priority; });
		ra::sort(g_Categories, [&](const std::unique_ptr<Category>& entry1, const std::unique_ptr<Category>& entry2) { return entry1->priority > entry2->priority; });

		for (const auto& entry : g_Icons) {
//			if (!entry->name.empty()) g_Keyrings.emplace_back(entry);
			if (entry->tag.empty())
			{
				categoryDefault = entry.get();
				Log(logLevel) << "ySI: Default category is '" + entry->filename + "'";
			}
//			Set(entry->tag, entry);
		}

		for (const auto& entry : g_Categories)
		{
			if (entry->IsKey()) g_Keyrings.emplace_back((Keyring*)entry.get());
			if (entry->IsInventory()) g_Tabline.emplace_back((Tab*)entry.get());
			g_StringToTabs.emplace(entry->tag, (Tab*) entry.get());
		}

		ra::sort(g_Tabline, [&](const Tab* entry1, const Tab* entry2) { return entry1->tabPriority > entry2->tabPriority; });
	}

	void DeferredInit()
	{
		Log(logLevel) << "Loading files for Sorting Icons";
		const auto dir = GetCurPath() + R"(\Data\menus\ySI)";
		const auto then = std::chrono::system_clock::now();
		if (!std::filesystem::exists(dir)) Log() << dir + " does not exist.";
		else for (const auto& iter : std::filesystem::directory_iterator(dir))
			if (iter.is_directory()) Log(logLevel) << iter.path().string() + " found";
			else if (iter.path().extension().string() == ".json") Files::HandleJSON(iter.path());
			else if (iter.path().extension().string() == ".xml") Files::HandleXML(iter.path());
		ProcessEntries();
		const auto now = std::chrono::system_clock::now();
		const auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - then);
		Log(logLevel) << std::format("Loaded items, categories and tabs in {:d} ms", diff.count());
	}

	extern void Init()
	{
		Commands::Register();
		if (g_nvseInterface->isEditor) return;
		HandleINI();

		Patch::AlterSorting(enable && bSort);
		Patch::AddIcons(enable && bIcons);
		Patch::ReplaceHotkeyIcons(enable && bHotkeys);
		Patch::AddKeyrings(enable && bCategories);
		Patch::AddPromptIcon(enable && bPrompt);
//		Patch::AddTabs(true);

		deferredInit.emplace_back(CraftingComponents::Fill);
		if (bSort || bIcons || bHotkeys || bCategories) deferredInit.emplace_back(DeferredInit);
//		if (bCategories) mainLoop.emplace_back(Keyrings::KeyringRefreshPostStewie);
		if (bIcons) mainLoopDoOnce.emplace_back(Icons::InjectTemplates);
	}
}
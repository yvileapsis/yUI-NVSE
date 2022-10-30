#include "SI.h"

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

		logLevel = ini.GetOrCreate("Sorting and Icons", "iDebug", 0, "; debug output for Sorting and Icons, useful for developers");

		bSort = ini.GetOrCreate("Sorting and Icons", "bSortInventory", 1, "; sort inventory according to tag names supplied in .json");
		bIcons = ini.GetOrCreate("Sorting and Icons", "bAddInventoryIcons", 1, "; add ycons to inventory, container and barter menus");
		bHotkeys = ini.GetOrCreate("Sorting and Icons", "bReplaceHotkeyIcons", 1, "; replace hotkey icons with ycons");
		bCategories = ini.GetOrCreate("Sorting and Icons", "bEnableCategories", 1, "; enable keyring-like clickable categories (this destroys vanilla keyring, so you have to have .json files supplying a new keyring category, i.e. ySI.json)");

		ini.SaveFile(iniPath.c_str(), false);
	}

	void ProcessEntries()
	{
		ra::sort(g_Items, [&](const ItemPtr& entry1, const ItemPtr& entry2) { return entry1->common.priority > entry2->common.priority; });
		ra::sort(g_Categories, [&](const CategoryPtr& entry1, const CategoryPtr& entry2) { return entry1->priority > entry2->priority; });
		ra::sort(g_Tabs, [&](const TabPtr& entry1, const TabPtr& entry2) { return entry1->priority > entry2->priority; });

		for (const auto& entry : g_Categories) {
			if (!entry->name.empty()) g_Keyrings.emplace_back(entry);
			if (entry->tag.empty()) categoryDefault = entry;
			g_StringToCategory.emplace(entry->tag, entry);
		}

		for (const auto& entry : g_Tabs)
		{
			if (entry->inventory) g_Tabline.emplace_back(entry);
			g_StringToTabs.emplace(entry->tab, entry);
		}

		ra::sort(g_Tabline, [&](const TabPtr& entry1, const TabPtr& entry2) { return entry1->tabPriority > entry2->tabPriority; });
	}

	void DeferredInit()
	{
		Log("Loading files", kToLog | logLevel);
		const auto dir = GetCurPath() + R"(\Data\menus\ySI)";
		const auto then = std::chrono::system_clock::now();
		if (!std::filesystem::exists(dir)) Log(dir + " does not exist.", kToLog | logLevel);
		else for (const auto& iter : std::filesystem::directory_iterator(dir))
			if (iter.is_directory()) Log(iter.path().string() + " found");
			else if (iter.path().extension().string() == ".json") Files::HandleJSON(iter.path());
			else if (iter.path().extension().string() == ".xml") Files::HandleXML(iter.path());
		ProcessEntries();
		const auto now = std::chrono::system_clock::now();
		const auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - then);
		Log(FormatString("Loaded items, categories and tabs in %d ms", diff.count()), kToLog | logLevel);
	}

	extern void Init()
	{
		Commands::Register();
		if (g_nvseInterface->isEditor) return;
		HandleINI();

		Patches::AlterSorting(enable && bSort);
		Patches::AddIcons(enable && bIcons);
		Patches::ReplaceHotkeyIcons(enable && bHotkeys);
		Patches::AddKeyrings(enable && bCategories);
		Patches::AddPromptIcon();
//		Patches::AddTabs(true);

		deferredInit.emplace_back(CraftingComponents::Fill);
		if (bSort || bIcons || bHotkeys || bCategories) deferredInit.emplace_back(DeferredInit);
		if (bCategories) mainLoop.emplace_back(Keyrings::KeyringRefreshPostStewie);
		if (bIcons) mainLoopDoOnce.emplace_back(Icons::InjectTemplates);
	}
}
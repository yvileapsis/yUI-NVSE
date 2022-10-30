#include "SI.h"

#include <main.h>
#include <GameAPI.h>
#include <ranges>
#include <functions.h>
#include <SimpleINILibrary.h>

namespace SortingIcons
{
	void HandleINI()
	{
		CSimpleIniA ini;
		ini.SetUnicode();

		auto iniPath = GetCurPath() + yUI_INI;
		if (ini.LoadFile(iniPath.c_str()) == SI_FILE) return;

		enable = ini.GetOrCreate("General", "bSortingIcons", true, "; enable 'Sorting and Icons' feature. If required files are not found this will do nothing.");

		logLevel = ini.GetOrCreate("Sorting and Icons", "iDebug", 0, "; debug output for Sorting and Icons, useful for developers");

		bSort = ini.GetOrCreate("Sorting and Icons", "bSortInventory", 1, "; sort inventory according to tag names supplied in .json");
		bIcons = ini.GetOrCreate("Sorting and Icons", "bAddInventoryIcons", 1, "; add ycons to inventory, container and barter menus");
		bHotkeys = ini.GetOrCreate("Sorting and Icons", "bReplaceHotkeyIcons", 1, "; replace hotkey icons with ycons");
		bCategories = ini.GetOrCreate("Sorting and Icons", "bEnableCategories", 1, "; enable keyring-like clickable categories (this destroys vanilla keyring, so you have to have .json files supplying a new keyring category, i.e. ySI.json)");

		g_FixIndefiniteSorting = ini.GetOrCreate("General", "bFixIndefiniteSorting", 1, "; fix the issue where items with different conditions would 'jump around' on update");

		ini.SaveFile(iniPath.c_str(), false);
	}

	void ProcessSIEntries()
	{
		ra::sort(g_Items, [&](const Item& entry1, const Item& entry2)
		         { return entry1.common.priority > entry2.common.priority; });

		if (!bJustInTime)
		{
			for (const auto form : *GetAllForms()) {
				if (!form || !form->IsInventoryObjectAlt()) continue;
				Files::AssignCategoryToItem(form);
			}
//			SIFiles::g_Items_JSON = std::vector<SIItem>();
		}

		ra::sort(g_Categories, [&](const Category& entry1, const Category& entry2)
		         { return entry1.priority > entry2.priority; });

		for (auto& entry : g_Categories) {
			if (!entry.name.empty()) g_Keyrings.emplace(entry.tag);
			g_StringToCategory.emplace(entry.tag, std::move(entry));
		}
		g_Categories = std::vector<Category>();

		ra::sort(g_Tabs, [&](const Tab& entry1, const Tab& entry2)
			{ return entry1.priority > entry2.priority; });
		for (auto& entry : g_Tabs) g_StringToTabs.emplace(entry.tab, std::move(entry));

		std::vector<Tab> vector;
		for (const auto& snd : g_StringToTabs | std::views::values) vector.push_back(snd);
		ra::sort(vector, [&](const Tab& entry1, const Tab& entry2)
			{ return entry1.tabPriority > entry2.tabPriority; });
		for (const auto& it : vector) if (it.inventory) g_Tabline.push_back(it.tab);

		g_Tabs = std::vector<Tab>();
	}

	void ProcessFiles()
	{
		Log("Loading files", kToLog | logLevel);
		const auto dir = GetCurPath() + R"(\Data\menus\ySI)";
		const auto then = std::chrono::system_clock::now();
		if (!std::filesystem::exists(dir)) Log(dir + " does not exist.", kToLog | logLevel);
		else for (const auto& iter : std::filesystem::directory_iterator(dir))
		{
			const auto& path = iter.path();
			if (iter.is_directory()) Log(path.string() + " found");
			else if (path.extension().string() == ".json") Files::HandleJson(path);
			else if (path.extension().string() == ".xml") Files::HandleXML(path);
		}
		ProcessSIEntries();
		const auto now = std::chrono::system_clock::now();
		const auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - then);
		Log(FormatString("Loaded items, categories and tabs in %d ms", diff.count()), kToLog | logLevel);
	}

	extern void Init()
	{
		Commands::Register();
		if (g_nvseInterface->isEditor) return;
		HandleINI();

		Patches::AlterSorting(g_FixIndefiniteSorting || (enable && bSort));
		Patches::AddIcons(enable && bIcons);
		Patches::ReplaceHotkeyIcons(enable && bHotkeys);
		Patches::AddKeyrings(enable && bCategories);
		Patches::AddPromptIcon();
//		Patches::AddTabs(true);

		deferredInit.emplace_back(CraftingComponents::Fill);
		if (bSort || bIcons || bHotkeys || bCategories) deferredInit.emplace_back(ProcessFiles);
		if (bCategories) mainLoop.emplace_back(Sorting::KeyringRefreshPostStewie);
		if (bIcons) mainLoopDoOnce.emplace_back(Icons::InjectTemplates);
	}
}
